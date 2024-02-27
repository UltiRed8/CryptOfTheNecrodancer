#include "Path.h"
#include "Macro.h"

Path::Path(const Vector2i& _start, const Vector2i& _end)
{
	start = _start;
	end = _end;
	width = Random(3, 1);
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
	while (start.x != end.x)
	{
		int _directionX = end.x - start.x;
		start.x += (_directionX > 0) ? 1 : -1;
		tiles.push_back(new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y)));
		for (int _index = 0; _index < width; _index++)
		{
			tiles.push_back(new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x, (start.y + _index) * TILE_SIZE.y)));
		}
	}
}

void Path::FixY()
{
	while (start.y != end.y)
	{
		int _directionY = end.y - start.y;
		start.y += (_directionY > 0) ? 1 : -1;
		tiles.push_back(new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y)));
		for (int _index = 0; _index < width; _index++)
		{
			tiles.push_back(new Tile("wall.png", Vector2f((start.x + _index) * TILE_SIZE.x, start.y * TILE_SIZE.y)));
		}
	}
}