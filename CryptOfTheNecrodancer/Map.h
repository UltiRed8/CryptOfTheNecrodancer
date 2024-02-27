#pragma once
#include "Room.h"
#include "Macro.h"
#include "Map.h"
#include "Singleton.h"

class Map : public Singleton<Map>
{
	vector<Room*> rooms;
	vector<Tile*> tiles;
	int tempoIndex;
	bool chainToggle;

public:
	Vector2i GetRandomRoomSize() const
	{
		const int _sizeX = Random(10, 5);
		const int _sizeY = Random(10, 5);

		return Vector2i(_sizeX, _sizeY);
	}

public:
	Map();
	Map(const int _roomCount);

public:
	void Generate(const int _roomCount);
	void InitMap(const int _roomCount);
	void ResetAllTilesColor();
	void InitTiles();
	void Update();
	void UpdateTilesColor();
	void SetAllTilesOriginColor();
};

