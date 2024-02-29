#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "MusicManager.h"
#include "AnimationComponent.h"
#include "RythmComponent.h"
#include "Wall.h"
#include "LightningManager.h"
#include "Stair.h"
#include "Door.h"
#include "Map.h"

#define PATH_PLAYER "PlayerSprite.png"

Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, "", _position)
{
	inventory = new Inventory();
	ressources = _ressources;
	MovementComponent* _movement = new MovementComponent(this);
	components.push_back(_movement);
	AnimationData _animation = AnimationData("Idle", Vector2f(0, 0),Vector2f(26,26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new RythmComponent(this, [this]() { GetComponent<MovementComponent>()->SetCanMove(true); },nullptr , [&]() { GetComponent<MovementComponent>()->SetCanMove(false); }));
	components.push_back(new AnimationComponent(this, PATH_PLAYER, { _animation }, ANIM_DIR_NONE));

	CollisionComponent* _collisions = new CollisionComponent(this);
	components.push_back(_collisions);
	components.push_back(new LightningComponent("PlayerLight", this, 350));

	_movement->InitCollisions(_collisions, {
		CollisionReaction(ET_WALL, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Wall* _wall = dynamic_cast<Wall*>(_entity);
			_wall->DestroyWall();
		}),
		CollisionReaction(ET_STAIR, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Map::GetInstance().NextLevel();
		}),
		CollisionReaction(ET_DOOR, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Door* _door = dynamic_cast<Door*>(_entity);
			_door->OpenDoor();
		}),
	});

	InitInput();
	zIndex = 1;
	chainMultiplier = new int(1);
	type = ET_PLAYER;
}

Player::~Player()
{
	delete inventory;
	delete chainMultiplier;
}

void Player::InitInput()
{
	new ActionMap("Mouvements",
		{ ActionData("Haut", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(0,-1)); }, {Event::KeyPressed, Keyboard::Up}),
		  ActionData("Bas", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(0, 1)); }, {Event::KeyPressed, Keyboard::Down}),
		  ActionData("Droite", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(1, 0)); }, {Event::KeyPressed, Keyboard::Right}),
		  ActionData("Gauche", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(-1, 0)); }, {Event::KeyPressed, Keyboard::Left})
		});

	// TODO remove
	new ActionMap("TempDebug",
		{ ActionData("Decrease", [this]() { *chainMultiplier = 1; cout << "Set chain multiplier to: 1!" << endl; }, {Event::KeyPressed, Keyboard::Num1}),
		  ActionData("Increase", [this]() { *chainMultiplier = 2; cout << "Set chain multiplier to: 2!" << endl; }, {Event::KeyPressed, Keyboard::Num2}),
		  ActionData("SpeedIncrease", [this]() { MusicManager::GetInstance().SpeedUp(); }, {Event::KeyPressed, Keyboard::Num3}),
		  ActionData("SpeedDecrease", [this]() { MusicManager::GetInstance().SpeedDown(); }, {Event::KeyPressed, Keyboard::Num4}),
		});
}

void Player::Update()
{
	Entity::Update();
	GetComponent<AnimationComponent>()->Update();
}
