#pragma once
#include "Entity.h"


class Tile : public Entity
{
	Shape* shape;
	vector<Entity*> entities;

public:
	Tile(const string& _id,const string& _tilePath,const Vector2f& _position);

	// Hérité via Entity
	void Update() override;
};

