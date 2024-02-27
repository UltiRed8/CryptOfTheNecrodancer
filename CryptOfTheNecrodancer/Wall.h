#pragma once
#include"Entity.h"
class Wall:public Entity
{
public:
	Wall(const string& _wallPath, const Vector2f& _position);

public:

	void Update() override;
};

