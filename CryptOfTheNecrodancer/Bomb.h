#pragma once
#include "Placeable.h"
#include "Timer.h"


class Bomb : public Placeable
{
	bool isActive;
	int cooldown;
public:
	Bomb(const Vector2f& pos, const bool _isActive);

public:
	void Explode();
	virtual void Update() override;
};

