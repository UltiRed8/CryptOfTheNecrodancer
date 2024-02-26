#include "Map.h"
#include "EntityManager.h"
#include "Player.h"


void Map::UpdateTilesColor()
{
	bool _hasChain = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier() > 1.0f;

	ResetAllTilesColor();


	if (!_hasChain)
	{
		for (Tile* _tile : tiles)
		{
			_tile->InvertColors();
		}
	}
	else
	{
		for (int _index = 0; _index < tiles.size(); _index++)
		{
			Shape* _shape = tiles[_index]->GetShape();
			const int _a = tempoIndex == 1 ? 255 : 200;
			if (_shape->getFillColor().a == _a)
			{
				Color _color;
				_color = tempoIndex == 1 ? Color(53, 233, 136, 255) : Color(255, 62, 216, 255);
				_shape->setOutlineColor(Color(0, 0, 0, 100));
				_shape->setOutlineThickness(-5.0f);
				_shape->setFillColor(_color);
			}
		}
	}

	//if (chainToggle && _hasChain)
	//{
	//	// activer les couleurs
	//	for (int _index = 0; _index < tiles.size(); _index++)
	//	{

	//	}
	//	chainToggle = false;
	//}
	//else if (!chainToggle && !_hasChain)
	//{
	//	// reset
	//	chainToggle = true;
	//}


	//for (Tile* _tile : tiles)
	//{
	//	_tile->InvertColors();
	//}

}



Map::Map(const int _roomCount)
{
	InitMap(_roomCount);
	InitTiles();
	tempoIndex = 1;
	SetAllTilesOriginColor();
	chainToggle = true;
}


void Map::InitMap(const int _roomCount)
{
	for (int _index = 0; _index < _roomCount; _index++)
	{
		rooms.push_back(new Room(GetRandomRoomSize(), { 0.f,0.f }));
	}
}

void Map::InitTiles()
{
	for (Room* _room : rooms)
	{
		for (Tile* _tile : _room->GetAllTiles())
		{
			tiles.push_back(_tile);
		}
	}
}

void Map::Update()
{
	if (tempoIndex == 3)
	{
		tempoIndex = 1;
	}
	UpdateTilesColor();
	tempoIndex++;
}

void Map::ResetAllTilesColor()
{
	for (Tile* _tile : tiles)
	{
		_tile->ResetColor();
	}
}

void Map::SetAllTilesOriginColor()
{
	const int _size = static_cast<const int>(tiles.size());
	for (int _index = 0; _index < _size; _index++)
	{
		Tile* _tile = tiles[_index];
		Shape* _shape = tiles[_index]->GetShape();
		const Vector2i& _position = Vector2i(_shape->getPosition());
		const Vector2i _tilePosition = Vector2i(_position.x / int(TILE_SIZE.x), _position.y / int(TILE_SIZE.y));
		if ((_tilePosition.x + _tilePosition.y) % 2 == 0)
		{
			_tile->SetColors(Color(135, 79, 2, 255), Color(135, 79, 2, 200));
		}

		else
		{
			_tile->SetColors(Color(135, 79, 2, 200), Color(135, 79, 2, 255));
		}
	}
}