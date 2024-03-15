#include "Trap.h"
#include "Macro.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "MusicManager.h"
#include "SoundManager.h"
#include "Bomb.h"
#include "CameraManager.h"

#define PATH_CONFUSE_TRAP "Dungeons/ConfuseTrap.png"
#define PATH_CONFUSE_TRAP_PRESSED "Dungeons/ConfuseTrapPressed.png"
#define PATH_FAST_TRAP "Dungeons/FastTrap.png"
#define PATH_FAST_TRAP_PRESSED "Dungeons/FastTrapPressed.png"
#define PATH_SLOW_TRAP "Dungeons/SlowTrap.png"
#define PATH_SLOW_TRAP_PRESSED "Dungeons/SlowTrapPressed.png"
#define PATH_BOMB_TRAP "Dungeons/BombTrap.png"
#define PATH_WATER_TRAP "Dungeons/Water.png"

#define SOUND_WATER_IN "Assets/Sounds/mov_water_in.ogg"
#define SOUND_WATER_OUT "Assets/Sounds/mov_water_out.ogg"
#define SOUND_TRAP "Assets/Sounds/obj_trap_trapdoor_open.ogg"

Trap::Trap(const Vector2f& _position, const TrapType& _trapType) : Placeable(STRING_ID("Trap"), "", _position)
{
	type = ET_TRAP;
	trapType = _trapType;
	callback = nullptr;
	pressedPath = "";
	normalPath = "";
	cooldown = 0;
	Init();
	if (_trapType == TR_WATER)
	{
		TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER_TRAP, Random(0, 2), Vector2i(26, 26));
	}
	else
	{
		TextureManager::GetInstance().Load(shape, normalPath);
		shape->setScale(0.5f, 0.5f);
		shape->move(Vector2f(0.25f, 0.25f) * TILE_SIZE);
	}
}

Trap::Trap(const Vector2f& _position) : Placeable(STRING_ID("Trap"), "", _position)
{
	type = ET_TRAP;
	trapType = (TrapType)(Random(4, 0));
	callback = nullptr;
	pressedPath = "";
	normalPath = "";
	cooldown = 0;
	Init();
	if (trapType == TR_WATER)
	{
		TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER_TRAP, Random(2, 0), Vector2i(26, 26));
	}
	else
	{
		TextureManager::GetInstance().Load(shape, normalPath);
		shape->setScale(0.5f, 0.5f);
		shape->move(Vector2f(0.25f, 0.25f) * TILE_SIZE);
	}
}

void Trap::Init()
{
	InitAllPaths();
	InitCallback();
}

void Trap::InitAllPaths()
{
	string _normalPaths[] = {
		PATH_CONFUSE_TRAP,
		PATH_FAST_TRAP,
		PATH_SLOW_TRAP,
		PATH_BOMB_TRAP,
		PATH_WATER_TRAP,
	};

	string _pressedPaths[] = {
		PATH_CONFUSE_TRAP_PRESSED,
		PATH_FAST_TRAP_PRESSED,
		PATH_SLOW_TRAP_PRESSED,
		PATH_BOMB_TRAP,
		PATH_WATER_TRAP,
	};

	normalPath = _normalPaths[trapType];
	pressedPath = _pressedPaths[trapType];
}

void Trap::InitCallback()
{
	vector<function<void()>> _callbacks = {
		[&]() {
			dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->SetIsConfuse(true);
			CameraManager::GetInstance().Get("PlayerCamera")->SetRotation((const float)(Random(360, 0) - 180));
			cooldown = 5;
		},
		[&]() { MusicManager::GetInstance().SpeedUp(); cooldown = 5; },
		[&]() { MusicManager::GetInstance().SpeedDown(); cooldown = 5; },
		[&]() { new Bomb(GetPosition() + Vector2f(-0.25f, -0.25f) * TILE_SIZE); cooldown = -1; },
		[&]() {
			Player* _player = (Player*)EntityManager::GetInstance().Get("Player");
			MovementComponent* _component = _player->GetComponent<MovementComponent>();
			if (_component->GetIsStun())
			{
				Destroy();
				_component->SetIsStun(false);
				SoundManager::GetInstance().Play(SOUND_WATER_OUT);
			}
			else
			{
				SoundManager::GetInstance().Play(SOUND_WATER_IN);
				_component->SetIsStun(true);
			}
		},
	};

	callback = _callbacks[trapType];
}

void Trap::Trigger()
{
	if (cooldown != 0) return;

	if (trapType != TR_WATER)
	{
		TextureManager::GetInstance().Load(shape, pressedPath);
		SoundManager::GetInstance().Play(SOUND_TRAP);
	}

	if (callback)
	{
		callback();
	}
}

void Trap::Update()
{
	if (cooldown > 0)
	{
		cooldown--;
		if (cooldown == 0)
		{
			if (trapType != TR_WATER)
			{
				TextureManager::GetInstance().Load(shape, normalPath);
			}
		}
	}
	Entity::Update();
}