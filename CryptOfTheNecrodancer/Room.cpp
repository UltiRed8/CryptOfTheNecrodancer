#include "Room.h"
#include "Macro.h"
Room::Room(const Vector2i& _size, const Vector2f& _startPosition, const vector<Vector2f>& _allTilesPosition)
{
	size = _size;
	startPosition = _startPosition;
	Init(_allTilesPosition);
}
 
void Room::Init(const vector<Vector2f>& _allTilesPosition)
{
	for (int _rowIndex = 0; _rowIndex < size.y; _rowIndex++)
	{
		vector<Tile*> _row;
		for (int _columnIndex = 0; _columnIndex < size.x; _columnIndex++)
		{
			const float x = startPosition.x + _columnIndex * TILE_SIZE.x;
			const float y = startPosition.y + _rowIndex * TILE_SIZE.y;


			const Vector2f _position = Vector2f(x, y);

			bool _isValidPos = true;
			for (Vector2f _tilePos : _allTilesPosition)
			{
				if (_tilePos == _position)
				{
					_isValidPos = false;
					break;
				}
			}

			if (!_isValidPos)
			{
				continue;
			}
			//Color(135, 79, 2, 255) maron
			//Color(255, 62, 216, 255) violet
			//Color(53,233,136,255) vert 
			//_tile->GetShape()->setOutlineColor(Color(0, 0, 0, 100));
			//_tile->GetShape()->setOutlineThickness(-5.0f);
			int _value = rand() % 100;

			if (_value > 80)
			{
				continue;
			}

		
			_row.push_back(new Tile("wall.png", _position));
		}
		tiles.push_back(_row);
	}
}
