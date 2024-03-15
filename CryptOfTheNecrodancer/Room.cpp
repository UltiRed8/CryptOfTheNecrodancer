#include "Room.h"
#include "Macro.h"
#include "Map.h"

Room::Room(const Vector2i& _size)
{
	if (_size == Vector2i(-1, -1))
	{
		size = GetRandomVector2i(4, 6);
	}
	else
	{
		size = _size;
	}
}

bool Room::Generate(vector<Vector2f>& _usedPositions)
{
	FloatRect _collisionRect;
	int _tries = 10;

	do
	{
		startPosition = Vector2f(GetRandomVector2i(0, 30)) * TILE_SIZE;
		_collisionRect = FloatRect(startPosition, Vector2f(size) * TILE_SIZE);
		_tries--;
	} while (Contains(_collisionRect, _usedPositions) && _tries >= 1);

	if (_tries != 0)
	{
		PlaceFloor(_usedPositions);
		return true;
	}

	return false;
}

void Room::PlaceFloor(vector<Vector2f>& _usedPositions)
{
	for (int _rowIndex = 0; _rowIndex < size.y; _rowIndex++)
	{
		for (int _columnIndex = 0; _columnIndex < size.x; _columnIndex++)
		{
			const float x = startPosition.x + _columnIndex * TILE_SIZE.x;
			const float y = startPosition.y + _rowIndex * TILE_SIZE.y;
			const Vector2f _position = Vector2f(x, y);
			floor.push_back(new Tile(_position));
			_usedPositions.push_back(_position);
		}
	}
}