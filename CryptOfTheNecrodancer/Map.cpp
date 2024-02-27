#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bat.h"
#include "Slime.h"
#include <functional>

using namespace std;

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

Map::Map()
{
	
}

void Map::Generate(const int _roomCount)
{
	InitMap(_roomCount);
	tempoIndex = 1;
	SetAllTilesOriginColor();
	new Path({ 0,0 }, { 10,10 });
	chainToggle = true;
}

void Map::InitMap(const int _roomCount)
{
	for (int _index = 0; _index < _roomCount; _index++)
	{
		rooms.push_back(new Room(GetRandomRoomSize(), Vector2f(GetRandomRoomPosition()),tilesPosition));
		UpdateTiles(rooms[_index]);
	}
	SpawnEnnemy();
}

void Map::UpdateTiles(const Room* _room)
{
	for (Tile* _tile : _room->GetAllTiles())
	{
		tiles.push_back(_tile);
		tilesPosition.push_back(_tile->GetShape()->getPosition());
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

void Map::SpawnEnnemy(const int _ennemyCount)
{
	/*vector<Enemy*> _enemyList =
	{
		new Bat(GetRandomTilePosition()),
		new GreenSlime(GetRandomTilePosition()),
		new BlueSlime(GetRandomTilePosition()),
		new OrangeSlime(GetRandomTilePosition()),
	};*/

	vector<function<void()>> _enemyList =
	{
	[this]() { new Bat(GetRandomTilePosition()); },
	[this]() { new GreenSlime(GetRandomTilePosition()); },
	[this]() { new BlueSlime(GetRandomTilePosition()); },
	[this]() { new OrangeSlime(GetRandomTilePosition()); },
	};

	int _randIndex;
	for (int _index = 0; _index < _ennemyCount; _index++)
	{
		_randIndex = Random(static_cast<int>(_enemyList.size()), 0);
		_enemyList[_randIndex]();
	}
}
