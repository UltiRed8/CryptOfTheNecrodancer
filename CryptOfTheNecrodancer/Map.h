#pragma once

#include "Room.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"
#include "Macro.h"
#include "Stair.h"
#include "Zones.h"

#include "NPC.h"

#include "Bat.h"
#include "Slime.h"
#include "Skeleton.h"

#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

class Map : public Singleton<Map>
{
	Zone preparedZone;
	Zone currentZone;
	int currentLevel;

	vector<Room*> rooms;
	vector<Path*> paths;

	vector<Tile*> floors;
	vector<Wall*> walls;
	vector<Entity*> others;
	vector<Stair*> stairs;

	Room* shop;

	int tempoIndex;
	bool chainToggle;
	bool isPurple;

	NPC* shopkeeper;

	string zoneFileName;

public:
	string GetZoneFileName() const
	{
		return zoneFileName.substr(0, zoneFileName.find_first_of('_'));
	}
	Vector2i GetRandomRoomPosition(const int _min = 0, const int _max = 30) const
	{
		Vector2i _pos = Vector2i(GetRandomVector2i(_min, _max));

		_pos.x *= (int)TILE_SIZE.x;
		_pos.y *= (int)TILE_SIZE.y;

		return _pos;
	}

	NPC* GetShopkeeper() const
	{
		return shopkeeper;
	}

	Zone GetCurrentZone() const
	{
		return currentZone;
	}

	void SetCurrentZone(const Zone& _currentZone)
	{
		currentZone = _currentZone;
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

private:
	void NextFloor();
	void LoadMap();
	void OpenLobby();

public:
	void Prepare(const Zone& _zoneToOpen);
	void Open(const Zone& _zoneToOpen);
	void OpenPrepared();
	




















private:
	void PrepareMap(const Zone& _currentZone);
	void UpdateZoneFileName();
	void Generate(const int _roomCount = 6, const int _amountOfEnemies = 25);
	void GenerateRooms(const int _roomCount);
	void UpdateDoors();
	void SetFloorColor(Tile* _floor, const bool _creation = false);
	
	void UpdateTilesColor();
	void GeneratePaths();
	void GenerateWalls();
	void GenerateShopRoom();
	void SetAllFloorOriginColor();
	void PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type);
	void GenerateDiamond(const int _diamondOnFloor = 1, int _diamondInWall = 2);
	void SpawnEnnemy(const int _ennemyCount = 10);
	void EraseOverlappings();
	void NextLevel();
	void NextMap();
	void PrepareMusic();
	void DeleteAll();

public:
	void Update();
	void AddOther(Entity* _entity);
	void AddFloorAt(const Vector2f& _position);



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