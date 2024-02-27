#pragma once
#include"Entity.h"
class Wall:public Entity
{
public:
	Wall(const Vector2f& _position);

public:

	void Update() override;
	Vector2f UpdatePosition(const Vector2f& _position);
};

