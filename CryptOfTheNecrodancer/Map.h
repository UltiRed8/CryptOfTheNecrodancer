#pragma once
#include "Room.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"
#include "Macro.h"

#include "Bat.h"
#include "Slime.h"
#include "Skeleton.h"

#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

class Map : public Singleton<Map>
{
	vector<Room*> rooms;
	vector<Entity*> tiles;
	vector<Tile*> shopTiles;
	vector<Vector2f> tilesPosition;
	vector<Path*> paths;
	int tempoIndex;
	bool chainToggle;
	bool isPurple;

public:
	Vector2i GetRandomRoomSize() const
	{
		const int _sizeX = Random(6, 4);
		const int _sizeY = Random(6, 4);

		return Vector2i(_sizeX, _sizeY);
	}

	Vector2i GetRandomRoomPosition() const
	{

		const int _x = Random(30, 0) * int(TILE_SIZE.x);
		const int  _y = Random(30, 0) * int(TILE_SIZE.y);

		return Vector2i(_x, _y);
	}

	Vector2f GetFirstTilePosition() const
	{
		return tiles[0]->GetShape()->getPosition();
	}

	Vector2f GetPositionOfRandomTileOfType(const EntityType& _type)
	{
		SetTilesPosition();
		vector<Vector2f> _position;
		for (Entity* _entity : tiles)
		{
			if (_entity->GetType() == _type)
			{
				_position.push_back(_entity->GetPosition());
			}
		}

		return _position[Random((int)_position.size() - 1)];
	}


	void SetTilesPosition() 
	{
		tilesPosition.clear();
		cout << tiles.size() << endl;
		for (Entity* _tile : tiles)
		{
			tilesPosition.push_back(_tile->GetPosition());
		}
	}

public:
	void Generate(const int _roomCount);
	void GenerateRooms(const int _roomCount);
	void Load(const string _path);
	void AddTileAt(const Vector2f& _position);

	void ResetAllTilesColor();
	void Update();
	void UpdateTilesColor();
	void EraseOverlappingTiles();
	void GeneratePaths();
	void GenerateWalls();
	void GenerateShopRoom();
	void SetAllTilesOriginColor();
	void PlaceWallsAroundRoom(Room* _room, const WallType& _type);
	void SpawnEnnemy(const int _ennemyCount = 10);
};

