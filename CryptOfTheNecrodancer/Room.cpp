#include "Room.h"
#include "Macro.h"
#include "Map.h"

#define PATH_FLOOR "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/floor.png"
#define PATH_WATER "Dungeons/Water.png"


Room::Room(const Vector2i& _size, const Vector2f& _startPosition)
{
	size = _size;
	startPosition = _startPosition;
	Generate();
}

void Room::Generate()
{
	for (int _rowIndex = 0; _rowIndex < size.y; _rowIndex++)
	{
		for (int _columnIndex = 0; _columnIndex < size.x; _columnIndex++)
		{
			const float x = startPosition.x + _columnIndex * TILE_SIZE.x;
			const float y = startPosition.y + _rowIndex * TILE_SIZE.y;
			const Vector2f _position = Vector2f(x, y);
			if (_columnIndex == 0)
			{
				Map::GetInstance().AddOther(new Tile(PATH_WATER, _position, ET_WATER));
			}
			else
			{
				floor.push_back(new Tile(PATH_FLOOR, _position));
			}

		}
	}
}