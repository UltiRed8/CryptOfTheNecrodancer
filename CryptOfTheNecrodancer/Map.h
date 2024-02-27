#pragma once
#include "Room.h"
#include "Map.h"
#include "Singleton.h"
#include "Path.h"
#include "Macro.h"


class Map : public Singleton<Map>
{
	vector<Room*> rooms;
	vector<Tile*> tiles;
	vector<Vector2f> tilesPosition;
	vector<Path*> paths;
	int tempoIndex;
	bool chainToggle;

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



	void SetTilesPosition() 
	{
		tilesPosition.clear();
		cout << tiles.size() << endl;
		for (Tile* _tile : tiles)
		{
			tilesPosition.push_back(_tile->GetShape()->getPosition());
		}
	}

public:
	void Generate(const int _roomCount);
	void GenerateRooms(const int _roomCount);

	void ResetAllTilesColor();
	void Update();
	void UpdateTilesColor();
	void EraseOverlappingTiles();
	void GeneratePaths();
	void GenerateWalls();
	void GenerateShopRoom();
	void SetAllTilesOriginColor();
	void PlaceWallsAroundRoom(Room* _room, const WallType& _type);
};

