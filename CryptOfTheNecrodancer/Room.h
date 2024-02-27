#pragma once
#include "Tile.h"

class Room
{
	Vector2i size;
	vector<vector<Tile*>> tiles;
	Vector2f startPosition;

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
		const int _randX = rand() % (tiles.size() - 1);
		const int _randY = rand() % (tiles[_randX].size() - 1);
		return tiles[_randX][_randY];
	}

public:
	void Init(const vector<Vector2f>& _allTilesPosition);
};

