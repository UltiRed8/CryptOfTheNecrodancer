#pragma once
#include "Room.h"
#include "Macro.h"

class Map
{
	vector<Room*> rooms;

public:
	Vector2i GetRandomRoomSize() const
	{
		const int _sizeX = Random(25, 5);
		const int _sizeY = Random(25, 5);

		return Vector2i(_sizeX, _sizeY);
	}

public:
	Map(const int _roomCount);

public:
	void InitMap(const int _roomCount);
};

