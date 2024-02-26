#include "Map.h"

Map::Map(const int _roomCount)
{
	InitMap(_roomCount);
}

void Map::InitMap(const int _roomCount)
{
	for (int _index = 0; _index < _roomCount; _index++)
	{
		rooms.push_back(new Room(GetRandomRoomSize(), { 0.f,0.f }));
	}
}
