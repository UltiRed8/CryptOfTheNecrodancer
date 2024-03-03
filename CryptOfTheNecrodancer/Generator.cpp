#include "Generator.h"
#include "MusicManager.h"
#include "LightningManager.h"
#include "Macro.h"
#include "Torch.h"
#include "Map.h"

#define PATH_SHOP_TILE "Dungeons/ShopTile.png"
#define PATH_UPGRADE_TILE "Dungeons/UpgradeTile.png"
#define PATH_FLOOR "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/floor.png"


Generator::Generator()
{
	zone = Z_ZONE1;
	rooms = vector<Room*>();
	paths = vector<Path*>();

	floors = vector<Tile*>();
	walls = vector<Wall*>();
	others = vector<Entity*>();
	stairs = vector<Stair*>();

	shop = nullptr;

	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;

	shopkeeper = nullptr;

	zoneFileName = string();
}

Generator::~Generator()
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

	for (Entity* _entity : others)
	{
		_entity->Destroy();
	}
	others.clear();

	for (Stair* _stair : stairs)
	{
		_stair->Destroy();
	}
	stairs.clear();

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
}

void Generator::Generate(const int _roomCount, const int _amountOfEnemies)
{
	GenerateRooms(_roomCount); 
	GeneratePaths();
	EraseOverlappings();
	GenerateShopRoom();
	GenerateWalls();
	EraseOverlappings();
	SetAllFloorOriginColor();
	UpdateDoors();
	SpawnEnnemy(_amountOfEnemies);
	GenerateDiamond();
}

void Generator::GenerateLobby()
{
	ifstream _in = ifstream("Assets/Saved/Lobby.txt");
	if (!_in)
	{
		cerr << "Erreur de chargement du lobby !" << endl;
		return;
	}

	map<char, function<void(const Vector2f& _position)>> _elements =
	{
		{ ' ', nullptr },
		{ '#', [this](const Vector2f& _position) { walls.push_back(new Wall(_position, WT_SHOP)); }},
		{ '.', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); }},
		{ 'S', [this](const Vector2f& _position) { stairs.push_back(new Stair(_position)); }},
		{ '3', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); others.push_back(new Door(_position)); }},
		{ 'E', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); others.push_back(new NPC(NPC_HEPHAESTUS, _position)); }},
		{ 'M', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); others.push_back(new NPC(NPC_MERLIN, _position));}},
		{ '2', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); /* TODO spawn item here (shops)*/ }},
		{ '1', [this](const Vector2f& _position) { others.push_back(new Tile(PATH_UPGRADE_TILE, _position)); }},
		{ 'P', [this](const Vector2f& _position) { floors.push_back(new Tile(PATH_FLOOR, _position)); EntityManager::GetInstance().Get("Player")->GetShape()->setPosition(_position); }},
		{ 'T', [this](const Vector2f& _position) { walls.push_back(new Wall(_position, WT_SHOP)); others.push_back(new Torch(_position)); }},
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

	stairs[0]->SetText("Zone 1");
	stairs[0]->SetZoneToLoad(Z_ZONE1);
	stairs[1]->SetText("Zone 2");
	stairs[1]->SetZoneToLoad(Z_ZONE2);
	stairs[2]->SetText("Soon(TM)");
	stairs[2]->SetLocked(LT_FORCE);

	SetAllFloorOriginColor();

	UpdateDoors();
}

void Generator::GenerateRooms(const int _roomCount)
{
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Room* _room = new Room(GetRandomVector2i(4, 6), Vector2f(GetRandomRoomPosition()));
		rooms.push_back(_room);
		const vector<Tile*>& _roomFloor = _room->GetFloor();
		floors.insert(floors.end(), _roomFloor.begin(), _roomFloor.end());
	}
}

void Generator::PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type)
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
			if (_wall->GetHasDiamond())
			{
				_allEntities.push_back(_wall);
			}
			else
			{
				_allEntities.push_back(_wall);
			}
		}

		_allPositionsAround = GetEmptyTilesAround(_allEntities);

		for (const Vector2f& _position : _allPositionsAround)
		{
			walls.push_back(new Wall(_position, (_index == _width - 1 && _finalDestructible) ? WT_INVULNERABLE : _type));
		}
	}
}

void Generator::GeneratePaths()
{
	const int _roomCount = (const int)rooms.size() - 1;
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

		const Vector2i& _startPosition = Vector2i((int)_startPositionX, (int)_startPositionY);
		const Vector2i& _endPosition = Vector2i((int)_endPositionX, (int)_endPositionY);

		Path* _path = new Path(_startPosition, _endPosition);

		vector<Tile*> _pathFloor = _path->GetFloor();
		paths.push_back(_path);
		floors.insert(floors.end(), _pathFloor.begin(), _pathFloor.end());
	}
}

void Generator::EraseOverlappings()
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

void Generator::GenerateShopRoom()
{
	const vector<Vector2f>& _availablePosition = GetEmptyTilesAround(floors);
	const Vector2f& _position = _availablePosition[Random((int)_availablePosition.size() - 1, 0)];

	shop = new Room(Vector2i(5, 7), _position);

	vector<Tile*>& _shopkeeperTiles = shop->GetFloor();
	const Vector2f& _shopkeeperPosition = _shopkeeperTiles[12]->GetPosition();
	shopkeeper = new NPC(NPC_SHOPKEEPER, _shopkeeperPosition);
	Tile* _first = _shopkeeperTiles[11];
	Tile* _second = _shopkeeperTiles[13];
	_first->SetTexture(PATH_SHOP_TILE);
	_second->SetTexture(PATH_SHOP_TILE);
	others.push_back(shopkeeper);
	others.push_back(_first);
	others.push_back(_second);

	new Pickable(5, PT_DIAMOND, STRING_ID("diamond"), _shopkeeperTiles[17]->GetPosition());

	const vector<Tile*>& _shopFloors = shop->GetFloor();
	floors.insert(floors.end(), _shopFloors.begin(), _shopFloors.end());

	PlaceWallsAroundFloor(_shopFloors, 1, false, WT_SHOP);

	EraseElement(floors, _first);
	EraseElement(floors, _second);

	rooms.push_back(shop);
}

void Generator::GenerateWalls()
{
	PlaceWallsAroundFloor(floors, 4, true, WT_DIRT);
}

void Generator::SetAllFloorOriginColor()
{
	const int _size = (const int)floors.size();
	for (int _index = 0; _index < _size; _index++)
	{
		SetFloorColor(floors[_index], true);
	}
}

void Generator::UpdateDoors()
{
	for (Entity* _entity : others)
	{
		if (Door* _door = dynamic_cast<Door*>(_entity))
		{
			_door->ComputeDirection();
		}
	}
}

void Generator::GenerateDiamond(const int _diamondOnFloor , int _diamondInWall )
{
	for (int _i = 0; _i < _diamondOnFloor; _i++)
	{
		new Pickable(1, PT_DIAMOND, STRING_ID("Diamond"), GetRandomElementInVector(floors)->GetPosition());
	}

	while (_diamondInWall >= 1)
	{
		Wall* _wall = GetRandomElementInVector(walls);
		if (_wall->GetWallType() == WT_DIRT)
		{
			_wall->SetHasDiamond(true);
			_diamondInWall--;
		}
	}
}

void Generator::SpawnEnnemy(const int _amountOfEnemies)
{
	vector<function<Entity* (const Vector2f& _position)>> _enemyList =
	{
		[this](const Vector2f& _position) { return new Bat(_position); },
		[this](const Vector2f& _position) { return new GreenSlime(_position); },
		[this](const Vector2f& _position) { return new BlueSlime(_position); },
		[this](const Vector2f& _position) { return new OrangeSlime(_position); },
		[this](const Vector2f& _position) { return new NormalSkeleton(_position); },
	};

	int _randIndex;
	vector<Vector2f> _positions = GetSpawnPositions();

	Vector2f _position = _positions[Random((int)_positions.size() - 1, 0)];
	if (_positions.empty()) return;
	for (int _index = 0; _index < _amountOfEnemies; _index++)
	{
		_position = _positions[Random((int)_positions.size() - 1, 0)];
		EraseElement(_positions, _position);
		_randIndex = Random(static_cast<int>(_enemyList.size() - 1), 0);
		others.push_back(_enemyList[_randIndex](_position));
	}
	_position = _positions[Random((int)_positions.size() - 1, 0)];
	EraseElement(_positions, _position);
	stairs.push_back(new Stair(_position, Map::GetInstance().GetCurrentZone()));
	_position = _positions[Random((int)_positions.size() - 1, 0)];
	EraseElement(_positions, _position);
	EntityManager::GetInstance().Get("Player")->GetShape()->setPosition(_position);
}

void Generator::AddFloorAt(const Vector2f& _position)
{
	Tile* _floor = new Tile(PATH_FLOOR, _position);
	floors.push_back(_floor);
	SetFloorColor(_floor, true);
}

void Generator::AddOther(Entity* _entity)
{
	others.push_back(_entity);
}

void Generator::UpdateTilesColor()
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

void Generator::Update()
{
	isPurple = !isPurple;
	if (tempoIndex >= 3)
	{
		tempoIndex = 1;
	}
	UpdateTilesColor();
	tempoIndex++;
}