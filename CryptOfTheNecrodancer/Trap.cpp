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
}

void Trap::Init()
{
	InitAllPaths();
	InitCallback();
}

void Trap::InitAllPaths()
{
	//TR_CONFUSION, TR_FAST, TR_SLOW, TR_BOMB, TR_DIRECTION
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
		[&]() {dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->SetIsConfuse(true); },
		[&]() {MusicManager::GetInstance().SpeedUp(); },
		[&]() {MusicManager::GetInstance().SpeedDown(); },
		[&]() { new Bomb(GetPosition()); },
	};

	callback = _callbacks[trapType];
}

void Trap::ExecuteTrap()
{
	if (callback)
	{
		callback();
	}
}

void Trap::Update(const float& _deltaTime)
{

}


