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

#define PATH_PLAYER "PlayerSprite.png"

Player::Player(const float _maxHp, const float _maxDammage, const string _id, const Vector2f& _position) : Living(_maxHp, _maxDammage,PATH_PLAYER,_id, _position)
{
	isConfuse = false;
	inventory = new Inventory();
	ressources = new PlayerRessource();
	alreadyMoved = false;
	zIndex = 1;
	chainMultiplier = new int(1);
	type = ET_PLAYER;
	components.push_back(new AnimationComponent(this, PATH_PLAYER, {
		AnimationData("Idle", Vector2f(0, 0), Vector2f(26, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f)
	}, ANIM_DIR_NONE));
	CollisionComponent* _collisions = new CollisionComponent(this);
	components.push_back(_collisions);
	MovementComponent* _movement = GetComponent<MovementComponent>();
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

		CollisionReaction(ET_PICKABLE, [this](Entity* _entity) {
			Pickable* _pickable = dynamic_cast<Pickable*>(_entity);
			_pickable->PickUp();
		}),

		CollisionReaction(ET_ENEMY, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			GetComponent<DamageComponent>()->Attack(_entity);
		}),

		CollisionReaction(ET_EPHAESTUS, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
		}),
	});

	new LightSource("PlayerLight", this, 350);

	InitInput();
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
				GetComponent<MovementComponent>()->SetDirection(Vector2i(0,-1) * GetConfusionEffect());
				if (!MusicManager::GetInstance().TriggerEvent())
				{
					GetComponent<MovementComponent>()->SetDirection(Vector2i(0,0));
				}
				alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Up}),

		  ActionData("HautR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Up}),



		  ActionData("Bas", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(0,1) * GetConfusionEffect());
				 if (!MusicManager::GetInstance().TriggerEvent())
				 {
					 GetComponent<MovementComponent>()->SetDirection(Vector2i(0, 0));
				 }
				 alreadyMoved = true;
			}; }, {Event::KeyPressed, Keyboard::Down}),

		  ActionData("BasR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Down}),



		  ActionData("Droite", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(1,0) * GetConfusionEffect());
				 if (!MusicManager::GetInstance().TriggerEvent())
				 {
					 GetComponent<MovementComponent>()->SetDirection(Vector2i(0, 0));
				 }
				 alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Right}),

		  ActionData("DroiteR", [this]() { alreadyMoved = false; }, {Event::KeyReleased, Keyboard::Right}),


		  ActionData("Gauche", [this]() { if (!alreadyMoved)
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(-1,0) * GetConfusionEffect());
				 if (!MusicManager::GetInstance().TriggerEvent())
				 {
					 GetComponent<MovementComponent>()->SetDirection(Vector2i(0, 0));
				 }
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
		  ActionData("temp1", [this]() { Map::GetInstance().NextMap(); }, {Event::KeyPressed, Keyboard::Num5}),
		});
}



void Player::Update()
{
	Entity::Update();
}

void Player::DieEvent()
{

}