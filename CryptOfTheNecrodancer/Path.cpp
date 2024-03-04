#include "Path.h"
#include "Macro.h"
#include "Map.h"

#define PATH_FLOOR "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/floor.png"

Path::Path(const Vector2i& _start, const Vector2i& _end, vector<Vector2f>& _usedPosition)
{
	start = _start;
	end = _end;
	width = 2;
	Generate(_usedPosition);
}

void Path::Generate(vector<Vector2f>& _usedPosition)
{
	bool _isYPriority = Random(1, 0);

	_isYPriority ? FixY(_usedPosition) : FixX(_usedPosition);
	!_isYPriority ? FixY(_usedPosition) : FixX(_usedPosition);
}

void Path::FixX(vector<Vector2f>& _usedPosition)
{
	while (start.x != end.x)
	{
		int _directionX = end.x - start.x;
		start.x += (_directionX > 0) ? 1 : -1;
		const Vector2f& _position = Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y);
		if (!Contains(_position, _usedPosition))
		{
			tiles.push_back(new Tile(PATH_FLOOR, _position));
			_usedPosition.push_back(_position);
		}
		for (int _index = 1; _index < width; _index++)
		{
			const Vector2f& _widthPosition = Vector2f(start.x * TILE_SIZE.x, (start.y + (_index * Random(1, 0) == 1 ? 1 : -1)) * TILE_SIZE.y);
			if (!Contains(_widthPosition, _usedPosition))
			{
				tiles.push_back(new Tile(PATH_FLOOR, _widthPosition));
				_usedPosition.push_back(_widthPosition);
			}
		}
	}
}

void Path::FixY(vector<Vector2f>& _usedPosition)
{
	while (start.y != end.y)
	{
		int _directionY = end.y - start.y;
		start.y += (_directionY > 0) ? 1 : -1;
		const Vector2f& _position = Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y);
		if (!Contains(_position, _usedPosition))
		{
			tiles.push_back(new Tile(PATH_FLOOR, _position));
			_usedPosition.push_back(_position);
		}
		for (int _index = 1; _index < width; _index++)
		{
			const Vector2f& _widthPosition = Vector2f((start.x + (_index * Random(1, 0) == 1 ? 1 : -1)) * TILE_SIZE.x, start.y * TILE_SIZE.y);
			if (!Contains(_widthPosition, _usedPosition))
			{
				tiles.push_back(new Tile(PATH_FLOOR, _widthPosition));
				_usedPosition.push_back(_widthPosition);
			}
		}
	}
}