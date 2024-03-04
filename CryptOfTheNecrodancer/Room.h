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
	vector<Tile*>& GetFloor()
	{
		return floor;
	}

public:
	Room(const Vector2i& _size = Vector2i(-1, -1));

private:
	void PlaceFloor(vector<Vector2f>& _usedPositions);

public:
	bool Generate(vector<Vector2f>& _usedPositions);
};