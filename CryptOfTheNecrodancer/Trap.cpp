#include "Trap.h"
#include "Macro.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "MusicManager.h"
#include "Bomb.h"

#define PATH_CONFUSE_TRAP "Entities/ConfuseTrap.png"
#define PATH_CONFUSE_TRAP_PRESSED "Entities/ConfuseTrapPressed.png"
#define PATH_FAST_TRAP "Entities/FastTrap.png"
#define PATH_FAST_TRAP_PRESSED "Entities/FastTrapPressed.png"
#define PATH_SLOW_TRAP "Entities/SlowTrap.png"
#define PATH_SLOW_TRAP_PRESSED "Entities/SlowTrapPressed.png"
#define PATH_BOMB_TRAP "Entities/BombTrap.png"

Trap::Trap(const Vector2f& _position, const TrapType& _trapType) : Placeable(STRING_ID("Trap"), "", _position)
{
	type = ET_TRAP;
	trapType = _trapType;
	callback = nullptr;
	pressedPath = "";
	normalPath = "";
	cooldown = 0;
	Init();
	TextureManager::GetInstance().Load(shape, normalPath);
	shape->setScale(0.5f, 0.5f);
}

Trap::Trap(const Vector2f& _position) : Placeable(STRING_ID("Trap"), "", _position)
{
	type = ET_TRAP;
	trapType = (TrapType)(Random(3, 0));
	callback = nullptr;
	pressedPath = "";
	normalPath = "";
	cooldown = 0;
	Init();
	TextureManager::GetInstance().Load(shape, normalPath);
	shape->setScale(0.5f, 0.5f);
}

void Trap::Init()
{
	InitAllPaths();
	InitCallback();
	shape->move(Vector2f(0.25f, 0.25f) * TILE_SIZE);
}

void Trap::InitAllPaths()
{
	string _normalPaths[] = {
		PATH_CONFUSE_TRAP,
		PATH_FAST_TRAP,
		PATH_SLOW_TRAP,
		PATH_BOMB_TRAP,
	};

	string _pressedPaths[] = {
		PATH_CONFUSE_TRAP_PRESSED,
		PATH_FAST_TRAP_PRESSED,
		PATH_SLOW_TRAP_PRESSED,
		PATH_BOMB_TRAP,
	};

	normalPath = _normalPaths[trapType];
	pressedPath = _pressedPaths[trapType];
}

void Trap::InitCallback()
{
	vector<function<void()>> _callbacks = {
		[&]() { dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->SetIsConfuse(true); cooldown = 5; },
		[&]() { MusicManager::GetInstance().SpeedUp(); cooldown = 5; },
		[&]() { MusicManager::GetInstance().SpeedDown(); cooldown = 5; },
		[&]() { new Bomb(GetPosition() + Vector2f(-0.25f, -0.25f) * TILE_SIZE); cooldown = -1; },
	};

	callback = _callbacks[trapType];
}

void Trap::Trigger()
{
	if (cooldown != 0) return;

	TextureManager::GetInstance().Load(shape, pressedPath);

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
			TextureManager::GetInstance().Load(shape, normalPath);
		}
	}
	Entity::Update();
}