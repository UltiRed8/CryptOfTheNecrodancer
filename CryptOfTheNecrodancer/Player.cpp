#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "MusicManager.h"
#include "AnimationComponent.h"
#include "Wall.h"
#include "SoundManager.h"
#include "LightningManager.h"
#include "Stair.h"
#include "Trap.h"
#include "Door.h"
#include "Map.h"
#include "MenuManager.h"
#include "Heart.h"
#include "WindowManager.h"
#include "Ice.h"
#include "HotCoals.h"
#include "Bomb.h"
#include "Swipe.h"
#include "Chest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#define SAVE_DATA "Assets/Saved/PlayerStats.txt"
#define SAVE_PURCHASED "Assets/Saved/PurchasedItems.txt"
#define SAVE_PURCHASED_DEFAULT "Assets/Saved/DefaultItems.txt"

#define PATH_PLAYER "Entities/PlayerSprite.png"
#define PATH_SHADOW "Entities/Shadow.png"

#define SOUND_DEATH "Assets/Sounds/sfx_player_death_ST.ogg"

#define SOUND_CHAIN_START "Assets/Sounds/sfx_chain_groove_ST.ogg"
#define SOUND_CHAIN_FAIL "Assets/Sounds/sfx_chain_break_ST.ogg"

int Player::GetDigLevel() const
{
	if (Item* _item = inventory->GetSlot(ST_SHOVEL)->GetItem())
	{
		return _item->GetStats().digLevel;
	}
	return 0;
}

float Player::LoadHP()
{
	ifstream _stream = ifstream("Assets/Saved/PlayerStats.txt");
	if (_stream)
	{
		string _line;
		getline(_stream, _line);
		getline(_stream, _line);
		return stof(_line);
	}
}

Player::Player(const float _maxHp, const string _id, const Vector2f& _position) : Living(LoadHP(), 0.0f, PATH_SHADOW, _id, _position, false)
{
	visuals = new RectangleShape(TILE_SIZE);
	TextureManager::GetInstance().Load(visuals, PATH_PLAYER);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);
	isStun = false;
	isConfuse = false;
	pickupCooldown = false;
	inventory = new Inventory();
	inventory->Open();
	ressources = new PlayerRessource();
	alreadyMoved = false;
	zIndex = 3;
	chainMultiplier = new int(1);
	type = ET_PLAYER;
	shouldSave = true;
	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(26, 26), 0, 3, 0.1f, false),
		}, "Idle", visuals));
	CollisionComponent* _collisions = new CollisionComponent(this);
	components.push_back(_collisions);
	MovementComponent* _movement = GetComponent<MovementComponent>();
	_movement->InitCollisions(_collisions, {
		CollisionReaction(ET_WALL, [this](Entity* _entity) {
			Wall* _wall = dynamic_cast<Wall*>(_entity);
			
			if (_wall->DestroyWall(GetDigLevel()))
			{
				if (Item* _item = inventory->GetSlot(ST_SHOVEL)->GetItem())
				{
					if (_item->GetStats().preventMovement)
					{
						GetComponent<MovementComponent>()->UndoMove();
					}
					if (Item* _item = inventory->GetSlot(ST_SHOVEL)->GetItem())
					{
						const ItemStats& _stats = _item->GetStats();
						if (!_stats.swipePath.empty())
						{
							new Swipe(_stats.swipePath, _stats.swipeAmount, 1.0f, false);
						}
					}
					return true;
				}
			}
			GetComponent<MovementComponent>()->UndoMove();
			if (Item* _item = inventory->GetSlot(ST_SHOVEL)->GetItem())
			{
				const ItemStats& _stats = _item->GetStats();
				if (!_stats.swipePath.empty())
				{
					new Swipe(_stats.swipePath, _stats.swipeAmount, 1.0f, false);
				}
			}
			return true;
		}),
		CollisionReaction(ET_STAIR, [this](Entity* _entity) {
			if (Stair* _stair = dynamic_cast<Stair*>(_entity))
			{
				if (!_stair->OpenZone())
				{
					GetComponent<MovementComponent>()->UndoMove();
				}
			}
			return false;
		}),
		CollisionReaction(ET_DOOR, [this](Entity* _entity) {
			MovementComponent* _movement = GetComponent<MovementComponent>();
			_movement->UndoMove();
			Door* _door = dynamic_cast<Door*>(_entity);
			_door->OpenDoor();
			WindowManager::GetInstance().Shake(Vector2f(*_movement->GetDirection()));
			return true;
		}),
		CollisionReaction(ET_ICE, [this](Entity* _entity) {
			Ice* _ice = dynamic_cast<Ice*>(_entity);
			Slide();
			return true;
		}),
		CollisionReaction(ET_TRAP, [this](Entity* _entity) {
			dynamic_cast<Trap*>(_entity)->Trigger();
			return false;
		}),
		CollisionReaction(ET_NPC, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			return true;
		}),
		CollisionReaction(ET_CHEST, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			Chest* _chest = dynamic_cast<Chest*>(_entity);
			_chest->Open();
			return true;
		}),
		CollisionReaction(ET_ITEM, [this](Entity* _entity) {
			Item* _item = dynamic_cast<Item*>(_entity);
			if (!_item->IsInInventory() && !pickupCooldown)
			{
				if (!_item->PickUp())
				{
					GetComponent<MovementComponent>()->UndoMove();
					return false;
				}
				pickupCooldown = true;
			}
			return false;
		}),
		});

	new LightSource("PlayerLight", this, 350);

	InitInput();
	InitLife();

	UpdateDamageZone();
}

Player::~Player()
{
	if (shouldSave)
	{
		SavePlayerStatsData();
	}
	delete chainMultiplier;
	delete ressources;
	for (AttackZone* _zone : damageZone) 
	{
		delete _zone;
	}
}

bool Player::ResetChainMultiplier()
{
	if (*chainMultiplier >= 2)
	{
		*chainMultiplier = 1;
		SoundManager::GetInstance().Play(SOUND_CHAIN_FAIL);
		return true;
	}
	return false;
}

void Player::SavePlayerStatsData()
{
	ofstream _file(SAVE_DATA);
	if (_file)
	{
		_file << *ressources->GetDiamonds() << endl << GetComponent<LifeComponent>()->GetMaxHealth();
	}
	else
	{
		cerr << "Impossible d'ouvrir le fichier pour sauvegarder." << endl;
	}
}

void Player::DeleteSavePlayerStatsData()
{
	remove(SAVE_DATA);
}

void Player::DeleteSavePurchasedItems()
{
	ifstream _in = ifstream(SAVE_PURCHASED_DEFAULT);
	ofstream _out = ofstream(SAVE_PURCHASED);

	string _line;
	while (getline(_in, _line))
	{
		_out << _line << endl;
	}
}	

void Player::Slide()
{
	MovementComponent* _movement = GetComponent<MovementComponent>();
	Map::GetInstance().GetEntityAt(GetPosition())->GetType() == ET_ICE
		? _movement->SetDirection(_movement->GetOldDirection()) : _movement->SetDirection(Vector2i(0, 0));
	alreadyMoved = true;
}

void Player::InitInput()
{
	new ActionMap("Mouvements",
		{ ActionData("Haut", [this]() {
		if (!alreadyMoved && !MenuManager::GetInstance().BlockPlayer())
			{
				GetComponent<MovementComponent>()->SetDirection(Vector2i(0,-1));
				alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Up}),

		  ActionData("HautR", [this]() { alreadyMoved = false; GetComponent<MovementComponent>()->SetDirection(Vector2i(0,0)); }, {Event::KeyReleased, Keyboard::Up}),

		  ActionData("Bas", [this]() { if (!alreadyMoved && !MenuManager::GetInstance().BlockPlayer())
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(0,1));
				 alreadyMoved = true;
			}; }, {Event::KeyPressed, Keyboard::Down}),

		  ActionData("BasR", [this]() { alreadyMoved = false; GetComponent<MovementComponent>()->SetDirection(Vector2i(0,0)); }, {Event::KeyReleased, Keyboard::Down}),



		  ActionData("Droite", [this]() { if (!alreadyMoved && !MenuManager::GetInstance().BlockPlayer())
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(1,0));
				 alreadyMoved = true;
			}}, {Event::KeyPressed, Keyboard::Right}),

		  ActionData("DroiteR", [this]() { alreadyMoved = false; GetComponent<MovementComponent>()->SetDirection(Vector2i(0,0)); }, {Event::KeyReleased, Keyboard::Right}),


		  ActionData("Gauche", [this]() { if (!alreadyMoved && !MenuManager::GetInstance().BlockPlayer())
			{
				 GetComponent<MovementComponent>()->SetDirection(Vector2i(-1,0));
				 alreadyMoved = true;
			}; }, {Event::KeyPressed, Keyboard::Left}),

		  ActionData("GaucheR", [this]() { alreadyMoved = false; GetComponent<MovementComponent>()->SetDirection(Vector2i(0,0)); }, {Event::KeyReleased, Keyboard::Left}),
	});
	new ActionMap("Inventory", {
		ActionData("UseBomb", [this]() {
			if (Map::GetInstance().IsInLobby()) return;
			if (Item* _item = inventory->GetSlot(ST_BOMB)->GetItem())
			{
				_item->ExecuteCallback();
			}
		}, { Event::KeyPressed, Keyboard::RShift }),
		ActionData("UseFood1", [this]() {
			if (Item* _item = inventory->GetSlot(ST_FOOD_TOP)->GetItem())
			{
				_item->ExecuteCallback();
			}
		}, { Event::KeyPressed, Keyboard::Numpad1 }),
		ActionData("UseFood2", [this]() {
			if (Item* _item = inventory->GetSlot(ST_FOOD_DOWN)->GetItem())
			{
				_item->ExecuteCallback();
			}
		}, { Event::KeyPressed, Keyboard::Numpad0 }),
		ActionData("debug", [this]() {
			Map::GetInstance().OpenPrepared();
		}, { Event::KeyPressed, Keyboard::Num0 }),
	});
}

void Player::InitLife()
{
	life = new Menu("PlayerLife", {});

	LifeComponent* _lifeComp = GetComponent<LifeComponent>();

	const float _life = _lifeComp->GetMaxHealth();
	const int _heartsCount = (int)(_life / 100.0f);

	for (int _index = 0; _index < _heartsCount; _index++)
	{
		Heart* _heart = new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f,
			Vector2f(SCREEN_WIDTH - 55 * (3.8f + _index * 1.2f), SCREEN_HEIGHT - 55 * 12.3), H_FULL);
		_heart->SetOwner(life);
		_heart->Register();
	}

	life->Open();
}

void Player::UpdateLife()
{
	vector<Heart*> _hearts;

	for (UIElement* _element : life->GetAllValues())
	{
		Heart* _heart = dynamic_cast<Heart*>(_element);
		_hearts.push_back(_heart);
	}

	float _lifeCopy = *GetComponent<LifeComponent>()->GetCurrentHealth();

	for (int _index = (int)_hearts.size() - 1; _index >= 0; _index--)
	{
		Heart* _heart = _hearts[_index];
		_lifeCopy -= 100.0f;
		if (_lifeCopy <= -100.0f)
		{
			_heart->SetState(H_EMPTY);
		}
		else if (_lifeCopy <= -50.0f)
		{
			_heart->SetState(H_HALF);
		}
		else
		{
			_heart->SetState(H_FULL);
		}

		_heart->UpdateLife();
	}
}

void Player::UpdateHeartAnimation()
{
	heartIndex--;
	if (heartIndex < 0)
	{
		heartIndex = (int)(life->GetAllValues().size() - 1);
	}
	dynamic_cast<Heart*>(life->GetAllValues()[heartIndex])->UIHeart();
}

void Player::UpdateDamageZone()
{
	for (AttackZone* _zone : damageZone)
	{
		delete _zone;
	}
	damageZone.clear();

	Item* _weapon = inventory->GetSlot(ST_ATTACK)->GetItem();
	if (_weapon)
	{
		for (const InteractionData& _data : _weapon->GetStats().attackRange)
		{
			FloatRect _rect = _data.rect * TILE_SIZE;

			RectangleShape* _shape = new RectangleShape(_rect.getSize());
			_shape->setPosition(GetPosition() + _rect.getPosition());
			_shape->setOrigin(-_rect.getPosition() + TILE_SIZE / 2.0f);

			Color _color = _data.isRed ? Color::Red : Color::Yellow;
			_color.a = 0;
			_shape->setFillColor(_color);

			damageZone.push_back(new AttackZone(_rect.getPosition(), _shape, _data.isRed));
		}
	}
}

void Player::Update()
{
	map<string, int> _rotations = {
		{ "01", 90 },
		{ "0-1", 270 },
		{ "10", 0 },
		{ "-10", 180 },
	};

	const Vector2i& _direction = *GetComponent<MovementComponent>()->GetDirection();

	const string& _value = to_string(_direction.x) + to_string(_direction.y);

	const int _rotation = _rotations[_value];

	for (AttackZone* _zone : damageZone)
	{
		Shape* _shape = _zone->shape;
		_shape->setPosition(GetPosition() + TILE_SIZE / 2.0f);
		_shape->setRotation((const float) _rotation);
	}

	pickupCooldown = false;

	if (AnimationComponent* _animationComponent = GetComponent<AnimationComponent>())
	{
		_animationComponent->GetCurrent()->Replay();
	}

	for (Component* _component : components)
	{
		if ((MovementComponent*)_component)
		{
			continue;
		}
		_component->Update();
	}

	visuals->setPosition(shape->getPosition() + Vector2f(0.0f, -0.5f) * TILE_SIZE);

	if (!GetPressingKeys()) return;

	if (!TryToAttack())
	{
		GetComponent<MovementComponent>()->Update();
	}

	if (Item* _item = inventory->GetSlot(ST_HEAD)->GetItem())
	{
		if (Armor* _armor = (Armor*) _item)
		{
			if (_armor->GetArmorType() == AT_HEAD_MINERSCAP)
			{
				_armor->ExecuteCallback();
			}
		}
	}

	visuals->setPosition(shape->getPosition() + Vector2f(0.0f, -0.5f) * TILE_SIZE);
}

bool Player::TryToAttack()
{
	bool _limiter = true;

	for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
	{
		if (_entity->GetType() == ET_ENEMY)
		{
			for (AttackZone* _zone : damageZone)
			{
				if (!_zone->isRed) continue;
				if (_entity->GetShape()->getGlobalBounds().intersects(_zone->shape->getGlobalBounds()))
				{
					for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
					{
						if (_entity->GetType() == ET_ENEMY)
						{
							for (AttackZone* _zone : damageZone)
							{
								if (_zone->shape->getGlobalBounds().contains(_entity->GetPosition()))
								{
									if (Enemy* _enemy = (Enemy*)_entity)
									{
										if (_limiter)
										{
											_limiter = false;
											if (Item* _item = inventory->GetSlot(ST_ATTACK)->GetItem())
											{
												if (!_item->GetStats().preventMovement)
												{

													GetComponent<MovementComponent>()->Update();
												}
											}
											WindowManager::GetInstance().Shake(Vector2f(*GetComponent<MovementComponent>()->GetDirection()));
										}
										_enemy->Hit();
										Weapon* _weapon = (Weapon*)inventory->GetSlot(ST_ATTACK)->GetItem();
										const ItemStats& _stats = _weapon->GetStats();
										if (!_stats.swipePath.empty())
										{
											if (_weapon->weaponType == WT_STAFF || _weapon->weaponType == WT_STAFF_TITANIUM)
											{
												new Swipe(20, _stats.swipePath, _stats.swipeAmount);
											}
											else
											{
												new Swipe(_stats.swipePath, _stats.swipeAmount);
											}
										}
										if (GetComponent<DamageComponent>()->Attack(_entity))
										{
											if (*chainMultiplier <= 3)
											{
												*chainMultiplier += 1;
												if (*chainMultiplier == 2)
												{
													SoundManager::GetInstance().Play(SOUND_CHAIN_START);
												}
											}
										}

									}
								}
							}
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

void Player::DieEvent()
{
	SoundManager::GetInstance().Play(SOUND_DEATH);
	Map::GetInstance().ClearGenerator();
	Menu* _died = MenuManager::GetInstance().Get("Dead");
	_died->Open();
}


void Player::AddHeart(const int _amount)
{
	LifeComponent* _lifeComp = GetComponent<LifeComponent>();
	const int _lifeBefore = (const int) _lifeComp->GetMaxHealth();
	_lifeComp->SetMaxHealth(_lifeBefore + _amount * 100.0f);
	_lifeComp->SetCurrentHealth(_amount * 100.0f);
	const float _life = _lifeComp->GetMaxHealth();
	const int _heartsCount = (int)(_life / 100.0f - _lifeBefore / 100.0f);
	const int _maxHeartsCount = (int)(_life / 100.0f);
	
	Heart* _heart = new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * 3.8f, SCREEN_HEIGHT - 55 * 12.3), H_FULL);
	_heart->SetOwner(life);
	_heart->Register();

	vector<UIElement*> _elements = life->GetAllValues();
	int _index = 0;
	for (UIElement* _element : _elements)
	{
		if (Heart* _heart = dynamic_cast<Heart*>(_element))
		{
			_heart->GetShape()->setPosition(SCREEN_WIDTH - 55 * (3.8f + _index * 1.2f), SCREEN_HEIGHT - 55 * 12.3);
			_index++;
		}
	}
	if (_elements.size() >= 5) return;
	
	UpdateLife();
}