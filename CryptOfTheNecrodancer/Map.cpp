#include "Map.h"
#include "EntityManager.h"
#include "Player.h"

void Map::UpdateTilesColor()
{
	bool _hasChain = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier() > 1.0f;

	ResetAllTilesColor();

	if (!_hasChain)
	{
		for (Entity* _tile : tiles)
		{
			if (_tile->GetType() == ET_FLOOR)
			{
				Tile* _floor = dynamic_cast<Tile*>(_tile);
				_floor->InvertColors();
			}
		}
	}
	else
	{
		for (int _index = 0; _index < tiles.size(); _index++)
		{
			Entity* _entity = tiles[_index];
			if (_entity->GetType() != ET_FLOOR) continue;
			Shape* _shape = _entity->GetShape();
			const int _a = tempoIndex == 1 ? 255 : 200;
			if (_shape->getFillColor().a == _a)
			{
				Color _color;
				_color = tempoIndex == 1 ? Color(53, 233, 136, 255) : Color(255, 62, 216, 255);
				_shape->setOutlineColor(Color(0, 0, 0, 100));
				_shape->setOutlineThickness(-5.0f);
				_shape->setFillColor(_color);
			}
		}
	}
}

void Map::EraseOverlappingTiles()
{
	vector<Entity*> _overlappingTiles;
	//SetTilesPosition();
	for (Entity* _tile : tiles)
	{
		if (Contains<Vector2f>(_tile->GetShape()->getPosition(), tilesPosition))
		{
			/*tilesPosition.erase(_tile->GetShape()->getPosition())*/
			EraseElement(tilesPosition, _tile->GetShape()->getPosition());
		}

		else
		{
			_overlappingTiles.push_back(_tile);
		}
	}

	for (Entity* _tile : _overlappingTiles)
	{
		_tile->Destroy();
		EraseElement(tiles, _tile);
	}
}

void Map::GeneratePaths()
{
	cout << "Generating paths..." << endl;
	const int _roomCount = (int)rooms.size();
	for (int _index = 0; _index < _roomCount - 1; _index++)
	{
		Tile* _startTile = rooms[_index]->GetRandomTile();
		Tile* _endTile = rooms[_index + 1]->GetRandomTile();

		Shape* _startShape = _startTile->GetShape();
		Shape* _endShape = _endTile->GetShape();

		const float _startPositionX = _startShape->getPosition().x / TILE_SIZE.x;
		const float _startPositionY = _startShape->getPosition().y / TILE_SIZE.y;

		const float _endPositionX = _endShape->getPosition().x / TILE_SIZE.x;
		const float _endPositionY = _endShape->getPosition().y / TILE_SIZE.y;

		Vector2i _startPosition = Vector2i((int)_startPositionX,(int)_startPositionY);
		Vector2i _endPosition = Vector2i((int)_endPositionX, (int)_endPositionY);

		Path* _path = new Path(_startPosition, _endPosition);
		vector<Tile*> _pathTiles = _path->GetTiles();
		paths.push_back(_path);
		tiles.insert(tiles.end(), _pathTiles.begin(), _pathTiles.end());
	}
}

void Map::GenerateWalls()
{
	cout << "Generating walls..." << endl;
	vector<Vector2i> _posToCheck = {
		Vector2i(-1, -1),
		Vector2i(0, -1),
		Vector2i(1, -1),
		Vector2i(-1, 0),
		Vector2i(1,0),
		Vector2i(-1,1),
		Vector2i(0,1),
		Vector2i(1,1)
	};

	for (int _index = 0; _index < 4; _index++)
	{
		vector<Vector2f> _wallPosition;
		for (const Vector2f& _tilesPos : tilesPosition)
		{
			for (const Vector2i& _offset : _posToCheck)
			{
				Vector2f _newTilePos = _tilesPos;
				const Vector2f& _tileOffset = Vector2f(_offset.x * TILE_SIZE.x, _offset.y * TILE_SIZE.x);
				_newTilePos += _tileOffset;
				if (!Contains<Vector2f>(_newTilePos, tilesPosition))
				{
					if (_index == 3)
					{
						tiles.push_back(new Wall(_newTilePos, WT_INVULNERABLE));
					}
					else
					{
						tiles.push_back(new Wall(_newTilePos,WT_DIRT));
					}
					_wallPosition.push_back(_newTilePos);
				}
			}
		}
		tilesPosition.insert(tilesPosition.end(), _wallPosition.begin(), _wallPosition.end());
		_wallPosition.clear();
	}
}

void Map::GenerateShopRoom()
{
	vector<Vector2i> _posToCheck = {
		Vector2i(-1, -1),
		Vector2i(0, -1),
		Vector2i(1, -1),
		Vector2i(-1, 0),
		Vector2i(1,0),
		Vector2i(-1,1),
		Vector2i(0,1),
		Vector2i(1,1)
	};

	vector<Vector2f> _availablePosition;
	for (const Vector2f& _tilesPos : tilesPosition)
	{
		for (const Vector2i& _offset : _posToCheck)
		{
			Vector2f _newTilePos = _tilesPos;
			const Vector2f& _tileOffset = Vector2f(_offset.x * TILE_SIZE.x, _offset.y * TILE_SIZE.x);
			_newTilePos += _tileOffset;
			if (!Contains<Vector2f>(_newTilePos, tilesPosition))
			{
				_availablePosition.push_back(_newTilePos);
			}
		}
	}

	const Vector2f& _position = _availablePosition[rand() % _availablePosition.size() - 1];

	tilesPosition.push_back(_position);
	Room* _room = new Room(Vector2i(5,7), _position);
	vector<Tile*> _tiles = _room->Generate();
	tiles.insert(tiles.end(), _tiles.begin(), _tiles.end());

	for (Tile* _tile : _tiles)
	{
		tilesPosition.push_back(_tile->GetPosition());
	}


	rooms.push_back(_room);
	PlaceWallsAroundRoom(_room,WT_SHOP);
}

void Map::Generate(const int _roomCount)
{
	GenerateRooms(_roomCount);
	GeneratePaths();
	SetTilesPosition();
	GenerateShopRoom();
	GenerateWalls();
	EraseOverlappingTiles();
	SetAllTilesOriginColor();
	tempoIndex = 1;
	chainToggle = true;
}

void Map::GenerateRooms(const int _roomCount)
{
	cout << "Placing rooms..." << endl;
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Room* _room = new Room(GetRandomRoomSize(), Vector2f(GetRandomRoomPosition()));
		rooms.push_back(_room);
	}

	cout << "Generating rooms" << endl;

	for (Room* _room : rooms)
	{
		vector<Tile*> _floor = _room->Generate();
		tiles.insert(tiles.end(), _floor.begin(), _floor.end());
	}
}

void Map::Load(const string _path)
{
	ifstream _in = ifstream(_path);

	if (!_in)
	{
		cerr << "Erreur de chargement de " << _path << " !" << endl;
		return;
	}

	map<char, function<void(const Vector2f& _position)>> _elements = {
		{ '#', [this](const Vector2f& _position) { new Wall(_position, WT_SHOP); }},
		{ ' ', [this](const Vector2f& _position) { tiles.push_back(new Tile("wall.png", _position)); }},
		{ '.', nullptr },
	};

	string _line;
	Vector2i _tilePosition = Vector2i(0, 0);
	while (getline(_in, _line))
	{
		for (const char _char : _line)
		{
			function<void(const Vector2f& _position)> _callback = _elements[_char];
			if (_callback)
			{
				float _positionX = (float)_tilePosition.x * TILE_SIZE.x;
				float _positionY = (float)_tilePosition.y * TILE_SIZE.y;
				_callback(Vector2f(_positionX, _positionY));
			}
			_tilePosition.x += 1;
		}
		_tilePosition.x = 0;
		_tilePosition.y += 1;
	}

	SetTilesPosition();
	SetAllTilesOriginColor();
}

void Map::AddTileAt(const Vector2f& _position)
{
	Tile* _tile = new Tile("wall.png", _position);
	tiles.push_back(_tile);
	const Vector2i _tilePosition = Vector2i(_position.x / int(TILE_SIZE.x), _position.y / int(TILE_SIZE.y));
	if ((_tilePosition.x + _tilePosition.y) % 2 == 0)
	{
		if (!isPurple) _tile->SetColors(Color(135, 79, 2, 255), Color(135, 79, 2, 200));
		else _tile->SetColors(Color(135, 79, 2, 200), Color(135, 79, 2, 255));
	}
	else
	{
		if (!isPurple) _tile->SetColors(Color(135, 79, 2, 200), Color(135, 79, 2, 255));
		else _tile->SetColors(Color(135, 79, 2, 255), Color(135, 79, 2, 200));
	}
}

void Map::Update()
{
	isPurple = !isPurple;
	if (tempoIndex == 3)
	{
		tempoIndex = 1;
	}
	UpdateTilesColor();
	tempoIndex++;
}

void Map::ResetAllTilesColor()
{
	for (Entity* _tile : tiles)
	{
		if (_tile->GetType() == ET_FLOOR)
		{
			Tile* _floor = dynamic_cast<Tile*>(_tile);
			_floor->ResetColor();
		}
	}
}

void Map::SetAllTilesOriginColor()
{
	const int _size = static_cast<const int>(tiles.size());
	for (int _index = 0; _index < _size; _index++)
	{
		Entity* _entity = tiles[_index];
		if (_entity->GetType() != ET_FLOOR) continue;
		Tile* _tile = dynamic_cast<Tile*>(_entity);
		Shape* _shape = _entity->GetShape();
		const Vector2i& _position = Vector2i(_shape->getPosition());
		const Vector2i _tilePosition = Vector2i(_position.x / int(TILE_SIZE.x), _position.y / int(TILE_SIZE.y));
		if ((_tilePosition.x + _tilePosition.y) % 2 == 0)
		{
			_tile->SetColors(Color(135, 79, 2, 255), Color(135, 79, 2, 200));
		}

		else
		{
			_tile->SetColors(Color(135, 79, 2, 200), Color(135, 79, 2, 255));
		}
	}
}

void Map::PlaceWallsAroundRoom(Room* _room,const WallType& _type)
{
	cout << "Generating walls..." << endl;


	vector<Vector2f> _tilesPosition;

	for (const Tile* _tile : _room->GetFloor())
	{
		_tilesPosition.push_back(_tile->GetPosition());
	}
	

	vector<Vector2i> _posToCheck = {
		Vector2i(-1, -1),
		Vector2i(0, -1),
		Vector2i(1, -1),
		Vector2i(-1, 0),
		Vector2i(1,0),
		Vector2i(-1,1),
		Vector2i(0,1),
		Vector2i(1,1)
	};

	vector<Vector2f> _wallPosition;
	for (const Vector2f& _tilesPos : _tilesPosition)
	{
		for (const Vector2i& _offset : _posToCheck)
		{
			Vector2f _newTilePos = _tilesPos;
			const Vector2f& _tileOffset = Vector2f(_offset.x * TILE_SIZE.x, _offset.y * TILE_SIZE.x);
			_newTilePos += _tileOffset;
			if (!Contains<Vector2f>(_newTilePos, _tilesPosition))
			{
				new Wall(_newTilePos,_type);
				_wallPosition.push_back(_newTilePos);
			}
		}
	}
	tilesPosition.insert(tilesPosition.end(), _wallPosition.begin(), _wallPosition.end());
	_wallPosition.clear();

}
