#pragma once
#include "Tile.h"
#include "Wall.h"
#include "Macro.h"

class Room
{
	Vector2f startPosition;
	Vector2i size;
	vector<Tile*> floor;

public:
	Room(const Vector2i& _size,const Vector2f& _startPosition);

public:
	vector<Tile*> GetFloor() const
	{
		return floor;
	}

public:
	void Generate();
};

