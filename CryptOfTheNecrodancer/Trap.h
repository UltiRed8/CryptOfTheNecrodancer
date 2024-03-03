#pragma once
#include "Timer.h"
#include "Placeable.h"

enum TrapType
{
	TR_CONFUSION, TR_FAST, TR_SLOW, TR_BOMB
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
	Trap(const Vector2f& _position, const TrapType& _trapType);
	Trap(const Vector2f& _position);

private:
	void Init();
	void InitAllPaths();
	void InitCallback();

public:
	void Trigger();
	virtual void Update() override;
};