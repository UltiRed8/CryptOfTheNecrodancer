#pragma once
#include"Enemy.h"
class Bat:public Enemy
{

public:
	Bat(const Vector2f& _position);

public:
	void DieEvent() override;
	void SelectDirection();
	void UpdateRythm();
};

