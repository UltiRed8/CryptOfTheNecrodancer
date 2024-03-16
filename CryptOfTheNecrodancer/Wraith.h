#pragma once
#include "Enemy.h"

#define PATH_WRAITH "Entities/wraith.png"

class Wraith : public Enemy
{
public:
	Wraith(const Vector2f& _position);

public:
	void SelectDirection();
};

