#pragma once
#include "Timer.h"
#include "Placeable.h"

enum TrapType
{
	TR_CONFUSION, TR_FAST, TR_SLOW, TR_BOMB, TR_DIRECTION
};

enum DirectionTrap
{
	TR_DIR_NONE, TR_DIR_UP, TR_DIR_DOWN, TR_DIR_LEFT, TR_DIR_RIGHT
};

class Trap : public Placeable
{
	TrapType trapType;
	string pressedPath;
	string normalPath;
	function<void()> callback;
	bool isPressed;
	int cooldown;

public:

	string GetPathWithType(const TrapType& _trapType) const 
	{
	
	}

public:
	Trap(const Vector2f& _position, const TrapType& _trapType);

private:
	void Init();
	void InitAllPaths();
	void InitCallback();

public:
	void ExecuteTrap();
	virtual void Update(const float& _deltaTime);
};

