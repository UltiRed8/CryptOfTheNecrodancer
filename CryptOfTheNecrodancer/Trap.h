#pragma once
#include "Entity.h"
#include "Timer.h"


#define PATH_CONFUSE_TRAP "Entities/ConfuseTrap.png"
#define PATH_CONFUSE_TRAP_PRESSED "Entities/ConfuseTrapPressed.png"

#define PATH_FAST_TRAP "Entities/FastTrap.png"
#define PATH_FAST_TRAP_PRESSED "Entities/FastTrapPressed.png"

#define PATH_SLOW_TRAP "Entities/SlowTrap.png"
#define PATH_SLOW_TRAP_PRESSED "Entities/SlowTrapPressed.png"

#define PATH_BOMB_TRAP "Entities/BombTrap.png"


enum TrapType
{
	TR_CONFUSION, TR_FAST, TR_SLOW, TR_BOMB, TR_DIRECTION
};

enum DirectionTrap
{
	TR_DIR_NONE, TR_DIR_UP, TR_DIR_DOWN, TR_DIR_LEFT, TR_DIR_RIGHT
};

class Trap : public Entity
{

	int beatBeforeEndOfEffect; 
	TrapType trapType;
public:
	Trap(const string& _path, const Vector2f& _position, const TrapType& _trapType);

public:
	void ExecuteTrap(Entity* _entity);
	virtual void Update(const float& _deltaTime);
};

