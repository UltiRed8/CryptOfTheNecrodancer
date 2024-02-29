#pragma once
#include "Room.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"
#include "Macro.h"
#include "Stair.h"

#include "Bat.h"
#include "Slime.h"
#include "Skeleton.h"

#include "Shopkeeper.h"

#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

class Map : public Singleton<Map>
{
	vector<Room*> rooms;
	vector<Path*> paths;

	vector<Tile*> floors;
	vector<Wall*> walls;
	vector<Entity*> others; // doors, stairs...

	Room* shop;

	int tempoIndex;
	bool chainToggle;
	bool isPurple;

public:
	Vector2i GetRandomRoomPosition(const int _min = 0, const int _max = 30) const
	{
		Vector2i _pos = Vector2i(GetRandomVector2i(_min, _max));

		_pos.x *= (int)TILE_SIZE.x;
		_pos.y *= (int)TILE_SIZE.y;

		return _pos;
	}

	Vector2f GetFirstTilePosition() const
	{
		if (floors.empty()) return Vector2f();
		return floors[0]->GetShape()->getPosition();
	}

	vector<Vector2f> GetSpawnPositions()
	{
		vector<Vector2f> _positions;
		for (Tile* _floor : floors)
		{
			_positions.push_back(_floor->GetPosition());
		}

		if (shop)
		{
			for (Tile* _floor : shop->GetFloor())
			{
				EraseElement(_positions, _floor->GetPosition());
			}
		}

		for (Wall* _wall : walls)
		{
			EraseElement(_positions, _wall->GetPosition());
		}

		return _positions;
	}
	Entity* GetEntityAt(const Vector2f& _position)
	{
		for (Wall* _wall : walls)
		{
			if (_wall->GetPosition() == _position)
			{
				return _wall;
			}
		}
		return nullptr;
	}

public:
	Map();
	~Map();

public:
	void Generate(const int _roomCount = 6, const int _amountOfEnemies = 25);
	void GenerateRooms(const int _roomCount);
	void Load(const string _path);
	void AddFloorAt(const Vector2f& _position);
	void UpdateDoors();
	void SetFloorColor(Tile* _floor, const bool _creation = false);
	void Update();
	void UpdateTilesColor();
	void GeneratePaths();
	void GenerateWalls();
	void GenerateShopRoom();
	void GenerateDiamonds(const int _quantity = 30);
	void SetAllFloorOriginColor();
	void PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type);
	void SpawnEnnemy(const int _ennemyCount = 10);
	void EraseOverlappings();
	void NextLevel();
	void DeleteAll();

	template <typename Type>
	vector<Vector2f> GetEmptyTilesAround(const vector<Type*>& _entities)
	{
		const vector<Vector2i>& _offsets = {
			Vector2i(-1, -1),
			Vector2i(0, -1),
			Vector2i(1, -1),
			Vector2i(-1, 0),
			Vector2i(1,0),
			Vector2i(-1,1),
			Vector2i(0,1),
			Vector2i(1,1),
		};

		vector<Vector2f> _availablePositions;

		vector<Vector2f> _allPositions = GetAllPositions(_entities);

		for (const Vector2f& _position : _allPositions)
		{
			for (const Vector2i& _currentOffset : _offsets)
			{
				Vector2f _newTilePos = _position;
				const Vector2f& _tileOffset = Vector2f(_currentOffset.x * TILE_SIZE.x, _currentOffset.y * TILE_SIZE.x);
				_newTilePos += _tileOffset;
				if (!Contains<Vector2f>(_newTilePos, _allPositions))
				{
					_availablePositions.push_back(_newTilePos);
				}
			}
		}

		return _availablePositions;
	}
};