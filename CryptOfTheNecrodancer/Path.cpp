#include "Path.h"

Path::Path(const Vector2i& _start, const Vector2i& _end, const vector<Vector2f>& _allTilesPos)
{
	tiles = vector<Tile*>();
	start = _start;
	end = _end;
	width = Random(3, 1);
	allTilesPos = _allTilesPos;
	chanceToPlaceWall = 8;
	chanceToBeCleanPath = 3;
	Generate();
}

void Path::Generate()
{
	bool _isYPriority = Random(1, 0);

	_isYPriority ? FixY() : FixX();
	!_isYPriority ? FixY() : FixX();
}

void Path::FixX()
{
	bool _isClean = Random(chanceToBeCleanPath, 0) == 0;
	while (start.x != end.x)
	{
		int _directionX = end.x - start.x;
		start.x += (_directionX > 0) ? 1 : -1;
		PlaceTile(Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y));
		for (int _index = 0; _index < width; _index++)
		{
			PlaceTile(Vector2f(start.x * TILE_SIZE.x, (start.y + _index) * TILE_SIZE.y));
		}
	}
}

void Path::FixY()
{
	bool _isClean = Random(chanceToBeCleanPath, 0) == 0;
	while (start.y != end.y)
	{
		int _directionY = end.y - start.y;
		start.y += (_directionY > 0) ? 1 : -1;
		PlaceTile(Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y));
		for (int _index = 0; _index < width; _index++)
		{
			PlaceTile(Vector2f((start.x + _index) * TILE_SIZE.x, start.y * TILE_SIZE.y));
		}
	}
}

void Path::PlaceTile(const Vector2f& _position)
{
	if (!Random(chanceToPlaceWall, 0))
	{
		// wall
	}
	else
	{
		bool _isValid = true;
		for (const Vector2f& _tilePos : allTilesPos)
		{
			if (_position == _tilePos)
			{
				_isValid = false;
			}
		}
		if (!_isValid)
		{
			return;
		}
		tiles.push_back(new Tile("wall.png", _position));
	}
}