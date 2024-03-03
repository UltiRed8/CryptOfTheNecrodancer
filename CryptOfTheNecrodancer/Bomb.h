#pragma once
#include "Placeable.h"
#include "Timer.h"

class Bomb : public Placeable
{
	int cooldown;

public:
	Bomb(const Vector2f& _position);

private:
	void Explode();
	virtual void Update() override;
};