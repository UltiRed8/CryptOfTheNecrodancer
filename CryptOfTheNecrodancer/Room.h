#pragma once
#include "Tile.h"
#include "Wall.h"
#include "Macro.h"

class Room
{
	Vector2i size;
	Vector2f startPosition;
	vector<Tile*> floor;

public:
	Room(const Vector2i& _size,const Vector2f& _startPosition);

public:
	
	Tile* GetRandomTile() const
	{
		const int _value = Random(floor.size() - 1, 0);

		return floor[_value];
	}

public:
	vector<Tile*> Generate();
};

