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
	void FixX(vector<Vector2f>& _usedPosition);
	void FixY(vector<Vector2f>& _usedPosition);

public:
	Path(const Vector2i& _start, const Vector2i& _end, vector<Vector2f>& _usedPosition);

public:
	void Generate(vector<Vector2f>& _usedPosition);
};

