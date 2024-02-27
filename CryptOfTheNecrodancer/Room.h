#pragma once
#include "Tile.h"
#include "Macro.h"
#include "Wall.h"

class Room
{
	Vector2i size;
	vector<vector<Tile*>> tiles;
	Vector2f startPosition;
	bool haveCreatedWalls;

public:
	Room(const Vector2i& _size,const Vector2f& _startPosition,const vector<Vector2f>& _allTilesPosition);

public:
	vector<Tile*> GetAllTiles() const
	{
		vector<Tile*> _tiles;

		for (vector<Tile*> _row : tiles)
		{
			for (Tile* _tile : _row)
			{
				_tiles.push_back(_tile);
			}
		}

		return _tiles;
	}

	vector<vector<Tile*>> GetRoom() const
	{
		return tiles;
	}

	Tile* GetRandomTile() const
	{
		const int _randX = Random((int)tiles.size(), 0);
		const int _randY = Random((int)tiles[_randX].size(), 0);
		return tiles[_randX][_randY];
	}

public:
	void Init(const vector<Vector2f>& _allTilesPosition);
	void RandomCreateWalls(const Vector2f& _position);
};

