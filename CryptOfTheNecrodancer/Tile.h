#pragma once
#include "Entity.h"


class Tile : public Entity
{
	vector<Entity*> entities;

public:
	Tile(const string& _tilePath,const Vector2f& _position);

	// Hérité via Entity
	void Update() override;
};

