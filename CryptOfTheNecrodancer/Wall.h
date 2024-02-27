#pragma once
#include"Entity.h"
class Wall:public Entity
{
	bool isInvulnerable;
public:
	Wall(const Vector2f& _position,bool _isInvulnerable);

public:

	void Update() override;
};

