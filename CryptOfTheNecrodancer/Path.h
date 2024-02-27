#pragma once
#include "Tile.h"
#include "Macro.h"
#include "Wall.h"

class Path
{
	vector<Tile*> tiles;
	Vector2i start;
	Vector2i end;
	vector<Vector2f> allTilesPos;
	int width;
	int chanceToPlaceWall;
	int chanceToBeCleanPath;

public:
	vector<Tile*> GetTiles() const
	{
		return tiles; 
	}

private:
	void FixX();
	void FixY();
	void PlaceTile(const Vector2f& _position);

public:
	Path(const Vector2i& _start, const Vector2i& _end,const vector<Vector2f>& _allTilesPos);

public:
	void Generate();
};

