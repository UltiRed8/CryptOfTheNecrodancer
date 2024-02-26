#pragma once
#include "Tile.h"

class Room
{
	Vector2i size;
	vector<vector<Tile*>> tiles;
	Vector2f startPosition;

public:
	Room(const Vector2i& _size,const Vector2f& _startPosition);

public:
	void Init();
};

