#pragma once

#include "Wall.h"
#include "Tile.h"
#include "Zones.h"
#include "Room.h"
#include "Path.h"
#include "EntityManager.h"
#include "Door.h"
#include "NPC.h"
#include "Stair.h"
#include "Slime.h"
#include "Item.h"
#include "Skeleton.h"
#include "Bat.h"
#include "UIText.h"
#include <string>
#include <fstream>
#include "Chest.h"

#define C_BROWN Color(135, 79, 2, 255)
#define C_LIGHT_BROWN Color(135, 79, 2, 200)

class Generator
{
	Zone zone;

	vector<Room*> rooms;
	vector<Path*> paths;

	vector<Tile*> floors;
	vector<Wall*> walls;
	vector<Wall*> shopWalls;
	vector<Entity*> others;
	vector<Stair*> stairs;
	vector<Item*> items;
	vector<Entity*> shopTiles;

	vector<Vector2f> usedPositions;
	vector<Vector2f> spawnablePositions;

	int generationIndex;
	UIText* loadingText;

	Room* shop;

	int tempoIndex;
	bool chainToggle;
	bool isPurple;

	NPC* shopkeeper;

	string zoneFileName;
	bool* discoModeEnabled;

public:
	vector<Vector2f> GetAllWallsAndFloorPositions() const
	{
		vector<Vector2f> _positions = GetAllPositions(floors);

		vector<Vector2f> _temp = GetAllPositions(walls);
		_positions.insert(_positions.end(), _temp.begin(), _temp.end());

		_temp = GetAllPositions(shopTiles);
		_positions.insert(_positions.end(), _temp.begin(), _temp.end());

		return _positions;
	}
	Vector2i GetRandomRoomPosition(const int _min = 0, const int _max = 30) const
	{
		Vector2i _pos = Vector2i(GetRandomVector2i(_min, _max));

		_pos.x *= (int)TILE_SIZE.x;
		_pos.y *= (int)TILE_SIZE.y;

		return _pos;
	}
	void SetFloorColor(Tile* _floor, const bool _creation)
	{
		bool _hasChain;
		if (_creation) _hasChain = false;
		else _hasChain = *dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier() > 1.0f;

		_floor->InvertAlpha(isPurple, _hasChain);
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

	NPC* GetShopkeeper() 
	{
		return shopkeeper;
	}

	vector<Entity*> GetOthers()
	{
		return others;
	}
	vector<Wall*>& GetWalls()
	{
		return walls;
	}
	vector<Tile*>& GetFloors()
	{
		return floors;
	}
	Entity* GetEntityAt(const Vector2f& _position)
	{
		vector<Entity*> _entities;
		for (Wall* _wall : walls)
		{
			_entities.push_back(_wall);
		}
		for (Tile* _floor : floors)
		{
			_entities.push_back(_floor);
		}

		for (Entity* _entity : _entities)
		{
			if (_entity->GetPosition() == _position)
			{
				return _entity;
			}
		}
		return nullptr;
	}

public:
	Generator(bool* _discoModeEnabled);
	~Generator();

private:
	void GenerateRooms(const int _roomCount);
	void GenerateFire();
	void GenerateIce();

	vector<Wall*> PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type);
	void GeneratePaths();
	void EraseOverlappings();
	void GetSpawnablePositions();
	void GenerateShopRoom();
	void GenerateWalls();
	void SetAllFloorOriginColor();
	void UpdateDoors();
	void GenerateDiamond(const int _diamondOnFloor = 1, int _diamondInWall = 2);
	void SpawnEnnemy(const int _amountOfEnemies);
	void UpdateTilesColor();
	void PlaceShopDoor();
	void PlaceTorches();
	void SpawnPlayer();
	void SpawnStairs();
	void SpawnTraps(const int _amount);
	void Enable3DEffect();
	void GenerateChest(const int _chestCount);

public:
	void GenUpdate();
	void Generate();
	void GenerateLobby();
	void AddFloorAt(const Vector2f& _position);
	void AddOther(Entity* _entity);
	void Update();
	void AddItem(Item* _item);
	
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