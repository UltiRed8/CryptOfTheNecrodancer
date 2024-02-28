#pragma once
#include "Tile.h"
#include "Wall.h"

class Path
{
	Vector2i start;
	Vector2i end;
	vector<Tile*> tiles;
	int width;

public:
	vector<Tile*> GetFloor()
	{
		return tiles; 
	}

private:
	void FixX();
	void FixY();

public:
	Path(const Vector2i& _start, const Vector2i& _end);

public:
	void Generate();
};

