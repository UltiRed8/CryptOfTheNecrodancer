#pragma once
#include "Tile.h"
#include "Macro.h"

class Path
{
	vector<Tile*> tiles;
	Vector2i start;
	Vector2i end;
	int width;
	int chanceToPlaceWall;
	int chanceToBeCleanPath;

private:
	void FixX();
	void FixY();
	void PlaceTile(const Vector2f& _position);

public:
	Path(const Vector2i& _start, const Vector2i& _end);

public:
	void Generate();
};

