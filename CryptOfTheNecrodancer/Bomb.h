#pragma once
#include "Placable.h"
#include "Timer.h"


class Bomb : public Placable
{
	int cooldown;
public:
	Bomb(const Vector2f& pos);

public:
	void Explode();
	virtual void Update() override;
};

