#pragma once
#include "Room.h"
#include "Macro.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"

class Map : public Singleton<Map>
{
	vector<Room*> rooms;
	vector<Tile*> tiles;
	vector<Vector2f> tilesPosition;
	int tempoIndex;
	bool chainToggle;

public:
	Vector2i GetRandomRoomSize() const
	{
		const int _sizeX = Random(10, 5);
		const int _sizeY = Random(10, 5);

		return Vector2i(_sizeX, _sizeY);
	}

	Vector2i GetRandomRoomPosition() const
	{
		const int _x = Random(20, 0) * int(TILE_SIZE.x);
		const int  _y = Random(20, 0) * int(TILE_SIZE.y);

		return Vector2i(_x, _y);
	}

	Vector2f GetFirstTilePosition() const
	{
		return tiles[0]->GetShape()->getPosition();
	}
	Vector2f GetRandomTilePosition() const
	{
		return tilesPosition[Random(static_cast<int>(tilesPosition.size()),0)];
	}

public:
	Map();
	Map(const int _roomCount);

public:
	void Generate(const int _roomCount);
	void InitMap(const int _roomCount);
	void ResetAllTilesColor();
	void UpdateTiles(const Room* _room);
	void Update();
	void UpdateTilesColor();
	void SetAllTilesOriginColor();
	void SpawnEnnemy(const int _ennemyCount = 10);
};

