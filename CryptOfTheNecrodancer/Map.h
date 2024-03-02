#pragma once

#include "Room.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"
#include "Macro.h"
#include "Stair.h"
#include "Zones.h"

#include "Bat.h"
#include "Slime.h"
#include "Skeleton.h"

#include "Shopkeeper.h"

#include <iostream>
#include <functional>
#include <fstream>

#include "Generator.h"

using namespace std;

class Map : public Singleton<Map>
{
	Zone preparedZone;
	Zone currentZone;
	int currentLevel;
	int tempoIndex;
	bool chainToggle;
	bool isPurple;
	string zoneFileName;
	Generator* generator;

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

	Shopkeeper* GetShopkeeper() const
	{
		return generator->GetShopkeeper();
	}

	Zone GetCurrentZone() const
	{
		return currentZone;
	}

	void SetCurrentZone(const Zone& _currentZone)
	{
		currentZone = _currentZone;
	}

	Entity* GetEntityAt(const Vector2f& _position)
	{
		for (Wall* _wall : generator->GetWalls())
		{
			if (_wall->GetPosition() == _position)
			{
				return _wall;
			}
		}
		return nullptr;
	}
	/*vector<Vector2f> GetSpawnPositions()
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
	}*/

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
	void ClearGenerator();
	




















private:

	void UpdateZoneFileName();

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