#include "Path.h"

Path::Path(const Vector2i& _start, const Vector2i& _end)
{
	start = _start;
	end = _end;
	width = Random(3, 1);
	chanceToPlaceWall = Random(100, 1);
	chanceToBeCleanPath = Random(100,1);
	Generate();
}

void Path::Generate()
{
	bool _isClean = chanceToBeCleanPath <= 33;
	bool _isYPriority = Random(1, 0);

	cout << "start y: " << start.y << " end y: " << end.y << endl;
	cout << "start x: " << start.x << " end x: " << end.x << endl;

	while (start.x != end.x)
	{
		int _directionX = end.x - start.x;
		start.x += (_directionX > 0) ? 1 : -1;
		new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y));
		for (int _index = 0; _index < width; _index++)
		{
			int _newPosY = start.y + Random(2, 0) ? 1 : -1;
			new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x,_newPosY * TILE_SIZE.y));
		}
	}

	cout << "start y: " << start.y << " end y: " << end.y << endl;
	cout << "start x: " << start.x << " end x: " << end.x << endl;


	while (start.y != end.y)
	{
		int _directionY = end.y - start.y;
		start.y += (_directionY > 0) ? 1 : -1;
		new Tile("wall.png", Vector2f(start.x * TILE_SIZE.x, start.y * TILE_SIZE.y));
		for (int _index = 0; _index < width; _index++)
		{
			int _newPosX = start.x + Random(2,0) ? 1 : -1;
			new Tile("wall.png", Vector2f(_newPosX * TILE_SIZE.x, start.y * TILE_SIZE.y));	
		}
	}

	cout << "start y: " << start.y << " end y: " << end.y << endl;
	cout << "start x: " << start.x << " end x: " << end.x << endl;




	
}
