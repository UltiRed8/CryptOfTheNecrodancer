#pragma once
#include "Room.h"
#include "Macro.h"

class Map
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
	Map(const int _roomCount);

public:
	void InitMap(const int _roomCount);
	void ResetAllTilesColor();
	void InitTiles();
	void Update();
	void UpdateTilesColor();
	void SetAllTilesOriginColor();
};

