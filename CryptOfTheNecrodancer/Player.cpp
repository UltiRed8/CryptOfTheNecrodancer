#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "MusicManager.h"
#include "AnimationComponent.h"
#include "RythmComponent.h"
#include "Wall.h"
#include "SoundManager.h"
#include "LightningManager.h"
#include "Stair.h"
#include "Door.h"
#include "Map.h"
#include "MenuManager.h"

#include "Coin.h"
#include "Diamond.h"

#define PATH_PLAYER "PlayerSprite.png"

Player::Player(const string _id, const Vector2f& _position) : Entity(_id, "", _position)
{
	inventory = new Inventory();
	ressources = new PlayerRessource();
	MovementComponent* _movement = new MovementComponent(this);
	components.push_back(_movement);
	AnimationData _animation = AnimationData("Idle", Vector2f(0, 0), Vector2f(26, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new RythmComponent(this, [this]() { GetComponent<MovementComponent>()->SetCanMove(true); }, nullptr, [&]() { GetComponent<MovementComponent>()->SetCanMove(false); }));
	components.push_back(new AnimationComponent(this, PATH_PLAYER, { _animation }, ANIM_DIR_NONE));

	alreadyMoved = false;

	CollisionComponent* _collisions = new CollisionComponent(this);
	components.push_back(_collisions);
	components.push_back(new LightningComponent("PlayerLight", this, 350));
	components.push_back(new LifeComponent(this, [this]() { cout << "Tu est mort ! " << endl; }, false, 100.f));
	components.push_back(new DamageComponent(this, 100.f));

	_movement->InitCollisions(_collisions, {
		CollisionReaction(ET_WALL, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Wall* _wall = dynamic_cast<Wall*>(_entity);
			_wall->DestroyWall();
		}),
		CollisionReaction(ET_STAIR, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			if (Map::GetInstance().GetCurrentZone() == CL_Lobby)
			{
				if (*ressources->GetDiamonds() == 0)
				{
					Map::GetInstance().NextMap();
				}
				else
				{
					Menu* _leaveMenu = MenuManager::GetInstance().Get("LeaveLobby");
					_leaveMenu->Open();
				}
			}
			else
			{
				Map::GetInstance().NextMap();
			}
		}),
		CollisionReaction(ET_DOOR, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Door* _door = dynamic_cast<Door*>(_entity);
			_door->OpenDoor();
		}),

		CollisionReaction(ET_COIN, [this](Entity* _entity) {
			Coin* _coins = dynamic_cast<Coin*>(_entity);
			_coins->PickUpCoin();
		}),
		CollisionReaction(ET_DIAMOND, [this](Entity* _entity) {
			Diamond* _diamond = dynamic_cast<Diamond*>(_entity);
			_diamond->PickUpDiamond();
		}),

		CollisionReaction(ET_ENEMY, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			GetComponent<DamageComponent>()->Attack(_entity);
			cout << _entity->GetID() << " was killed!" << endl;
		}),

		CollisionReaction(ET_EPHAESTUS, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
		}),
		});

	InitInput();
	zIndex = 1;
	chainMultiplier = new int(1);
	type = ET_PLAYER;

	// listener = new Listener();
}

Player::~Player()
{
	delete inventory;
	delete chainMultiplier;
	delete ressources;
}

void Player::InitInput()
{
	new ActionMap("Mouvements",
		{ ActionData("Haut", [this]() {
		if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(0,-1));
				 alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Up}),

		  ActionData("HautR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Up}),



		  ActionData("Bas", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(0,1));
				 alreadyMoved = true;
			}; }, {Event::KeyPressed, Keyboard::Down}),

		  ActionData("BasR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Down}),



		  ActionData("Droite", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(1,0));
				 alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Right}),

		  ActionData("DroiteR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Right}),


		  ActionData("Gauche", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(-1,0));
				 alreadyMoved = true;
			}; }, {Event::KeyPressed, Keyboard::Left}),

		  ActionData("GaucheR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Left}),

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
	/*Vector2f _playerPos = GetPosition();
	listener->setPosition(_playerPos.x, _playerPos.y, 0);*/
	GetComponent<AnimationComponent>()->Update();
}
