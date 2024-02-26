#include "Room.h"
#include "Macro.h"
Room::Room(const Vector2i& _size, const Vector2f& _startPosition)
{
	size = _size;
	startPosition = _startPosition;
	Init();
}
 
void Room::Init()
{
	for (int _rowIndex = 0; _rowIndex < size.y; _rowIndex++)
	{
		vector<Tile*> _row;
		for (int _columnIndex = 0; _columnIndex < size.x; _columnIndex++)
		{
			const float x = startPosition.x + _columnIndex * TILE_SIZE.x;
			const float y = startPosition.y + _rowIndex * TILE_SIZE.y;

			const Vector2f _position = Vector2f(x, y);
			//Color(135, 79, 2, 255) maron
			//Color(255, 62, 216, 255) violet
			//Color(53,233,136,255) vert 
			//_tile->GetShape()->setOutlineColor(Color(0, 0, 0, 100));
			//_tile->GetShape()->setOutlineThickness(-5.0f);

		
			_row.push_back(new Tile("Assets/Textures/wall.png", _position));
		}
		tiles.push_back(_row);
	}
}
