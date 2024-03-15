#pragma once

#include "Placeable.h"

class Tile : public Placeable
{
	vector<Entity*> entities;
	Color defaultColor;
	Color baseColor;
	bool status;

public:
	Tile(const Vector2f& _position,const EntityType& _type = ET_FLOOR);
	Tile(const string& _path, const Vector2f& _position,const EntityType& _type = ET_FLOOR);

public:
	void InvertAlpha(const bool _status, const bool _highlighted);
};

