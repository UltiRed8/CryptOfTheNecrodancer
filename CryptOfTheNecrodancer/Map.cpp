#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include "LightningManager.h"
#define PATH_STAIR "stairs.png"

#define C_BROWN Color(135, 79, 2, 255)
#define C_LIGHT_BROWN Color(135, 79, 2, 200)

void Map::UpdateTilesColor()
{
	const bool _hasChain = *dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier() > 1.0f;

	if (!_hasChain)
	{
		for (Tile* _floor : floors)
		{
			_floor->InvertAlpha(isPurple);
		}
	}
	else
	{
		for (Tile* _floor : floors)
		{
			_floor->ToggleHighlight(isPurple ? 255 : 200);
		}
	}
}

void Map::GeneratePaths()
{
	const int _roomCount = (const int) rooms.size() - 1;
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Tile* _startTile = GetRandomElementInVector(rooms[_index]->GetFloor());
		Tile* _endTile = GetRandomElementInVector(rooms[_index + 1]->GetFloor());

		Shape* _startShape = _startTile->GetShape();
		Shape* _endShape = _endTile->GetShape();

		const float _startPositionX = _startShape->getPosition().x / TILE_SIZE.x;
		const float _startPositionY = _startShape->getPosition().y / TILE_SIZE.y;

		const float _endPositionX = _endShape->getPosition().x / TILE_SIZE.x;
		const float _endPositionY = _endShape->getPosition().y / TILE_SIZE.y;

		const Vector2i& _startPosition = Vector2i((int)_startPositionX,(int)_startPositionY);
		const Vector2i& _endPosition = Vector2i((int)_endPositionX, (int)_endPositionY);

		Path* _path = new Path(_startPosition, _endPosition);

		vector<Tile*> _pathFloor = _path->GetFloor();
		paths.push_back(_path);
		floors.insert(floors.end(), _pathFloor.begin(), _pathFloor.end());
	}
}

void Map::GenerateWalls()
{
	PlaceWallsAroundFloor(floors, 4, true, WT_DIRT);
}

void Map::GenerateShopRoom()
{
	const vector<Vector2f>& _availablePosition = GetEmptyTilesAround(floors);

	const Vector2f& _position = _availablePosition[rand() % _availablePosition.size() - 1];

	shop = new Room(Vector2i(5,7), _position);

	const vector<Tile*>& _shopFloors = shop->GetFloor();
	floors.insert(floors.end(), _shopFloors.begin(), _shopFloors.end());

	rooms.push_back(shop);

	PlaceWallsAroundFloor(_shopFloors, 1, false, WT_SHOP);
}

Map::Map()
{
	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;
	shop = nullptr;
	stair = nullptr;
}

Map::~Map()
{
	for (Room* _room : rooms)
	{
		delete _room;
	}
	for (Path* _path : paths)
	{
		delete _path;
	}
}

void Map::Generate(const int _roomCount)
{
	//return;
	GenerateRooms(_roomCount);
	GeneratePaths();
	EraseOverlappings();
	GenerateShopRoom();
	GenerateWalls();
	EraseOverlappings();
	SetAllFloorOriginColor();
	LightningManager::GetInstance().Construct(GetAllPositions(floors));
	LightningManager::GetInstance().Construct(GetAllPositions(walls));
}

void Map::EraseOverlappings()
{
	vector<Vector2f> _allPositions = GetAllPositions(walls);

	vector<Entity*> _entities;
	for (Wall* _wall : walls)
	{
		_entities.push_back(_wall);
	}
	for (Tile* _floor : floors)
	{
		_entities.push_back(_floor);
	}

	for (const Vector2f& _position : GetAllPositions(floors))
	{
		_allPositions.push_back(_position);
	}

	vector<Entity*> _validEntities;

	for (Entity* _entity : _entities)
	{
		const Vector2f& _entityPosition = _entity->GetPosition();
		if (Contains<Vector2f>(_entityPosition, _allPositions))
		{
			_validEntities.push_back(_entity);
			EraseElement(_allPositions, _entityPosition);
		}
	}

	vector<Wall*> _wallsToRemove;
	for (Wall* _wall : walls)
	{
		Entity* _entity = _wall;
		if (!Contains<Entity*>(_entity, _validEntities))
		{
			_entity->Destroy();
			_wallsToRemove.push_back(_wall);
		}
	}
	EraseElements(walls, _wallsToRemove);

	vector<Tile*> _floorsToRemove;
	for (Tile* _floor : floors)
	{
		Entity* _entity = _floor;
		if (!Contains<Entity*>(_entity, _validEntities))
		{
			_entity->Destroy();
			_floorsToRemove.push_back(_floor);
		}
	}
	EraseElements(floors, _floorsToRemove);
}

void Map::GenerateRooms(const int _roomCount)
{
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Room* _room = new Room(GetRandomVector2i(4, 6), Vector2f(GetRandomRoomPosition()));
		rooms.push_back(_room);
		const vector<Tile*>& _roomFloor = _room->GetFloor();
		floors.insert(floors.end(), _roomFloor.begin(), _roomFloor.end());
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
		{ '#', [this](const Vector2f& _position) { walls.push_back(new Wall(_position, WT_SHOP)); }},
		{ ' ', [this](const Vector2f& _position) { floors.push_back(new Tile("floor.png", _position)); }},
		{ '.', nullptr },
		{ 'S', [this](const Vector2f& _position) { stair = new Stair(PATH_STAIR, _position); }},
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
				const float _positionX = (float)_tilePosition.x * TILE_SIZE.x;
				const float _positionY = (float)_tilePosition.y * TILE_SIZE.y;
				_callback(Vector2f(_positionX, _positionY));
			}
			_tilePosition.x += 1;
		}
		_tilePosition.x = 0;
		_tilePosition.y += 1;
	}

	SetAllFloorOriginColor();
}

void Map::AddFloorAt(const Vector2f& _position)
{
	Tile* _floor = new Tile("floor.png", _position);
	floors.push_back(_floor);
	SetFloorColor(_floor);
}

void Map::Update()
{
	isPurple = !isPurple;
	if (tempoIndex >= 3)
	{
		tempoIndex = 1;
	}
	UpdateTilesColor();
	tempoIndex++;
}

void Map::SetFloorColor(Tile* _floor, const bool _creation)
{
	const Vector2f& _position = _floor->GetPosition();
	const Vector2i& _tilePosition = Vector2i(_position.x / int(TILE_SIZE.x), _position.y / int(TILE_SIZE.y));
	const bool _posEven = (_tilePosition.x + _tilePosition.y) % 2 == 0;

	Color _baseColor;

	bool _hasChain;
	if (_creation) _hasChain = false;
	else _hasChain = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier() > 1.0f;

	_baseColor = _posEven ? C_LIGHT_BROWN : C_BROWN;

	_floor->SetColors(_baseColor);
	if (!_hasChain)
	{
		if (!isPurple)
		{
			_floor->InvertAlpha(false);
		}
	}
	else
	{
		_floor->ToggleHighlight(isPurple ? 255 : 200);
	}
}

void Map::SetAllFloorOriginColor()
{
	const int _size = (const int)floors.size();
	for (int _index = 0; _index < _size; _index++)
	{
		SetFloorColor(floors[_index], true);
	}
}

void Map::PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type)
{
	for (int _index = 0; _index < _width; _index++)
	{
		vector<Vector2f> _allPositionsAround;

		vector<Entity*> _allEntities;
		for (Tile* _floor : _floors)
		{
			_allEntities.push_back(_floor);
		}
		for (Wall* _wall : walls)
		{
			_allEntities.push_back(_wall);
		}

		_allPositionsAround = GetEmptyTilesAround(_allEntities);

		for (const Vector2f& _position : _allPositionsAround)
		{
			walls.push_back(new Wall(_position, (_index == _width-1 && _finalDestructible) ? WT_INVULNERABLE : _type));
		}
	}

	/*vector<Vector2f> _positions;
	for (const Tile* _floor : floors)
	{
		_tilesPosition.push_back(_floor->GetPosition());
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
				tiles.push_back(new Wall(_newTilePos,_type));
				_wallPosition.push_back(_newTilePos);
			}
		}
	}
	tilesPosition.insert(tilesPosition.end(), _wallPosition.begin(), _wallPosition.end());
	_wallPosition.clear();*/
}

void Map::SpawnEnnemy(const int _ennemyCount)
{
	vector<function<void(const Vector2f& _position)>> _enemyList =
	{
		[this](const Vector2f& _position) { new Bat(_position); },
		[this](const Vector2f& _position) { new GreenSlime(_position); },
		[this](const Vector2f& _position) { new BlueSlime(_position); },
		[this](const Vector2f& _position) { new OrangeSlime(_position); },
		[this](const Vector2f& _position) { new NormalSkeleton(_position); },
	};


	int _randIndex;
	vector<Vector2f> _positions = GetSpawnPositions();
	new Shopkeeper(_positions[0]);
	if (_positions.empty()) return;
	for (int _index = 0; _index < _ennemyCount; _index++)
	{
		const Vector2f& _position = _positions[Random((int)_positions.size() - 1, 0)];
		EraseElement(_positions, _position);
		_randIndex = Random(static_cast<int>(_enemyList.size() - 1), 0);
		_enemyList[_randIndex](_position);
	}
}

void Map::NextLevel()
{
	DeleteAll();
	Generate(6);
	
	EntityManager::GetInstance().Get("Player")->GetShape()->setPosition(GetFirstTilePosition());
}

void Map::DeleteAll()
{
	for (Tile* _floor : floors)
	{
		_floor->Destroy();
	}
	floors.clear();
	for (Wall* _wall : walls)
	{
		_wall->Destroy();
	}
	walls.clear();

	stair->Destroy();
	stair = nullptr;
	for (Room* _room : rooms)
	{
		delete _room;
	}
	rooms.clear();
	shop = nullptr;

	for (Path* _path : paths)
	{
		delete _path;
	}
	paths.clear();

	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;
}
