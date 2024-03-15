#include "Generator.h"
#include "MusicManager.h"
#include "LightningManager.h"
#include "Macro.h"
#include "Torch.h"
#include "Item.h"
#include "Map.h"
#include "Trap.h"
#include "Ice.h"
#include "HotCoals.h"
#include "ShopStand.h"
#include "Dummy.h"

#define PATH_SHOP_TILE "Dungeons/ShopTile.png"
#define PATH_UPGRADE_TILE "Dungeons/UpgradeTile.png"
#define PATH_WATER "Dungeons/Water.png"
#define FONT "Assets/Font/Font.ttf"

Generator::Generator(bool* _discoModeEnabled)
{
	zone = Z_ZONE1;
	rooms = vector<Room*>();
	paths = vector<Path*>();

	floors = vector<Tile*>();
	walls = vector<Wall*>();
	others = vector<Entity*>();
	stairs = vector<Stair*>();

	loadingText = nullptr;

	shop = nullptr;

	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;

	shopkeeper = nullptr;
	discoModeEnabled = _discoModeEnabled;

	zoneFileName = Map::GetInstance().GetZoneFileName();

	generationIndex = -1;
}

Generator::~Generator()
{
	for (Tile* _floor : floors)
	{
		_floor->Destroy();
	}

	for (Wall* _wall : walls)
	{
		_wall->Destroy();
	}

	for (Entity* _entity : others)
	{
		_entity->Destroy();
	}

	for (Stair* _stair : stairs)
	{
		_stair->Destroy();
	}

	for (Room* _room : rooms)
	{
		delete _room;
	}

	for (Path* _path : paths)
	{
		delete _path;
	}

	for (Item* _item : items)
	{
		_item->Destroy();
	}
}

void Generator::Generate()
{
	loadingText = dynamic_cast<UIText*>(MenuManager::GetInstance().Get("Loading")->Get("LoadingText"));

	MenuManager::GetInstance().ToggleLoading();

	generationIndex = 0;
}

void Generator::GenerateLobby()
{
	loadingText = dynamic_cast<UIText*>(MenuManager::GetInstance().Get("Loading")->Get("LoadingText"));
	loadingText->GetText()->setString("Opening lobby");

	MenuManager::GetInstance().ToggleLoading();
	new Timer(STRING_ID("Lobby"), [this]() {
		ifstream _in = ifstream("Assets/Saved/Lobby.map");
		if (!_in)
		{
			cerr << "Erreur de chargement du lobby !" << endl;
			return;
		}

		map<char, function<void(const Vector2f& _position)>> _elements =
		{
			{ ' ', nullptr },
			{ '#', [this](const Vector2f& _position) { walls.push_back(new Wall(_position, WT_SHOP, zoneFileName, false)); }},
			{ '.', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); }},
			{ 'S', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); stairs.push_back(new Stair(_position)); }},
			{ '3', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); others.push_back(new Door(_position)); }},
			{ 'E', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); others.push_back(new NPC(NPC_HEPHAESTUS, _position)); }},
			{ 'M', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); others.push_back(new NPC(NPC_MERLIN, _position)); }},
			{ 'D', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); others.push_back(new NPC(NPC_DUNGEONMASTER, _position)); }},
			{ '1', [this](const Vector2f& _position) { others.push_back(new Tile(PATH_UPGRADE_TILE, _position)); }},
			{ 'P', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); EntityManager::GetInstance().Get("Player")->GetShape()->setPosition(_position); }},
			{ 'T', [this](const Vector2f& _position) { walls.push_back(new Wall(_position, WT_SHOP, zoneFileName)); others.push_back(new Torch(_position)); }},
			{ 'H', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); ShopStand(_position, STT_HEARTS); }},
			{ 'I', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); ShopStand({ _position, _position + Vector2f(12.0f, 0.0f) * TILE_SIZE }, STT_LOBBY);}},
			{ 'N', [this](const Vector2f& _position) { floors.push_back(new Tile(_position)); others.push_back(new Dummy(_position));}},
			{ '8', [this](const Vector2f& _position) {
				floors.push_back(new Tile(_position));
				int _posY = 0;
				int _index = 0;
				for (int _i = 0; _i < 27; _i++)
				{
					items.push_back(new Weapon(static_cast<WeaponType>(_i), STRING_ID("Item"), _position + Vector2f((const float)(_index * 2), (const float)_posY) * TILE_SIZE, false, false));
					_index++;
					if (_index >= 7)
					{
						_index = 0;
						_posY += 2;
					}
				}
			}},
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
		stairs[2]->SetText("---Paid DLC---\nOnly for 9.99$\nDownload NOW!!");
		stairs[2]->SetLocked(LT_FORCE);

		SetAllFloorOriginColor();
		UpdateDoors();
		Enable3DEffect();
		MenuManager::GetInstance().ToggleLoading();
		Map::GetInstance().UpdateLights(50);
		CameraManager::GetInstance().Get("PlayerCamera")->SetCameraToPlayer();
		MusicManager::GetInstance().Play();
	}, seconds(2.0f), 1, true);
}

void Generator::Enable3DEffect()
{
	for (Wall* _wall : walls)
	{
		Entity* _entity = GetEntityAt(_wall->GetPosition() - Vector2f(0, -1) * TILE_SIZE);
		if (_entity)
		{
			if (_entity->GetType() == ET_FLOOR || _entity->GetType() == ET_ICE)
			{
				_wall->Enable3D();
			}
		}
	}

}

void Generator::GenerateChest(const int _chestCount)
{
	loadingText->GetText()->setString("Generating chests");
	for (int _index = 0; _index < _chestCount; _index++)
	{
		Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
		EraseElement(spawnablePositions, _position);
		others.push_back(new Chest(_position));
	}
}

void Generator::AddItem(Item* _item)
{
	items.push_back(_item);
}

void Generator::GenerateRooms(const int _roomCount)
{
	loadingText->GetText()->setString("Generating rooms");
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Room* _room = new Room();
		if (_room->Generate(usedPositions))
		{
			rooms.push_back(_room);
			const vector<Tile*>& _roomFloor = _room->GetFloor();
			floors.insert(floors.end(), _roomFloor.begin(), _roomFloor.end());
		}
		else
		{
			delete _room;
		}
	}
}

void Generator::GenerateFire()
{
	loadingText->GetText()->setString("Generating Water");

	for (Room* _room : rooms)
	{
		int _waterTileNumberPerRoom = Random(5, 0) + 1;

		for (int _i = 0; _i < _waterTileNumberPerRoom; _i++)
		{
			if (spawnablePositions.empty()) return;
			Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
			EraseElement(spawnablePositions, _position);
			Entity* _entity = GetEntityAt(_position);
			if (_entity)
			{
				if (Tile* _floor = dynamic_cast<Tile*>(_entity))
				{
					EraseElement(floors, _floor);
					EraseElement(_room->GetFloor(), _floor);
					_floor->Destroy();

				}
			}
			others.push_back(new HotCoals(_position));
		}
	}
}

void Generator::GenerateIce()
{
	loadingText->GetText()->setString("Generating Water");

	for (Room* _room : rooms)
	{
		int _waterTileNumberPerRoom = Random(5, 0) + 1;

		for (int _i = 0; _i < _waterTileNumberPerRoom; _i++)
		{
			if (spawnablePositions.empty()) return;
			Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
			EraseElement(spawnablePositions, _position);
			Entity* _entity = GetEntityAt(_position);
			if (_entity)
			{
				if (Tile* _floor = dynamic_cast<Tile*>(_entity))
				{
					EraseElement(floors, _floor);
					EraseElement(_room->GetFloor(), _floor);
					_floor->Destroy();

				}
			}
			others.push_back(new Ice(_position));
		}
	}
}

vector<Wall*> Generator::PlaceWallsAroundFloor(vector<Tile*> _floors, const int _width, const bool _finalDestructible, const WallType& _type)
{
	vector<Wall*> _wallsOfRoom;
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
			if (!Contains(_position, usedPositions))
			{
				Wall* _wall = new Wall(_position, (_index == _width - 1 && _finalDestructible) ? WT_INVULNERABLE : _type, zoneFileName);
				walls.push_back(_wall);
				_wallsOfRoom.push_back(_wall);
				usedPositions.push_back(_position);
			}
		}
	}
	return _wallsOfRoom;
}

void Generator::GeneratePaths()
{
	loadingText->GetText()->setString("Generating paths");

	const int _roomCount = (const int)rooms.size() - 1;
	for (int _index = 0; _index < _roomCount; _index++)
	{
		Vector2f _startPosition = GetRandomElementInVector(rooms[_index]->GetFloor())->GetPosition() / TILE_SIZE;
		Vector2f _endPosition = GetRandomElementInVector(rooms[_index + 1]->GetFloor())->GetPosition() / TILE_SIZE;

		_startPosition /= TILE_SIZE;
		_endPosition /= TILE_SIZE;

		Path* _path = new Path(Vector2i(_startPosition), Vector2i(_endPosition), usedPositions);
		paths.push_back(_path);

		const vector<Tile*>& _pathFloor = _path->GetFloor();
		floors.insert(floors.end(), _pathFloor.begin(), _pathFloor.end());
	}
}

void Generator::EraseOverlappings()
{
	loadingText->GetText()->setString("Erasing overlappings");

	vector<Vector2f> _allPositions = GetAllPositions(walls);

	for (const Vector2f& _position : GetAllPositions(floors))
	{
		_allPositions.push_back(_position);
	}

	vector<Entity*> _entities;
	for (Wall* _wall : walls)
	{
		_entities.push_back(_wall);
	}
	for (Tile* _floor : floors)
	{
		_entities.push_back(_floor);
	}

	vector<Entity*> _validEntities;

	for (Entity* _entity : _entities)
	{
		Vector2f _entityPosition = _entity->GetPosition();
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
	EraseElements(shopWalls, _wallsToRemove);

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
	if (shop)
	{
		EraseElements(shop->GetFloor(), _floorsToRemove);
	}
}

void Generator::GenerateShopRoom()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetComponent<LifeComponent>()->SetIsInvulnerable(true);

	loadingText->GetText()->setString("Generating shop room");

	shop = new Room(Vector2i(5, 7));
	shop->Generate(usedPositions);
	rooms.push_back(shop);

	Vector2f _shopkeeperPosition = usedPositions[12];
	shopkeeper = new NPC(NPC_SHOPKEEPER, _shopkeeperPosition);
	others.push_back(shopkeeper);

	vector<Tile*>& _shopFloors = shop->GetFloor();
	for (int _index = 0; _index < 2; _index++)
	{
		Entity* _entity = _shopFloors[11 + _index];
		_entity->SetTexture(PATH_SHOP_TILE);
		EraseElement(_shopFloors, (Tile*)_entity);
		others.push_back(_entity);
		shopTiles.push_back(_entity);
	}

	ShopStand(_shopFloors[12 + 8]->GetPosition(), STT_SHOPKEEPER);

	floors.insert(floors.end(), _shopFloors.begin(), _shopFloors.end());

	shopWalls = PlaceWallsAroundFloor(_shopFloors, 1, false, WT_SHOP);
}

void Generator::PlaceShopDoor()
{
	loadingText->GetText()->setString("Placing shop door");
	vector<Wall*> _availableWalls;

	for (Wall* _wall : shopWalls)
	{
		if (_wall->CouldBeDoor())
		{
			_availableWalls.push_back(_wall);
		}
	}

	Wall* _selectedWall = _availableWalls[Random((int)_availableWalls.size() - 1, 0)];

	others.push_back(new Door(_selectedWall->GetPosition()));
	AddFloorAt(_selectedWall->GetPosition());
	EraseElement(walls, _selectedWall);
	_selectedWall->Destroy();
}

void Generator::PlaceTorches()
{
	loadingText->GetText()->setString("Placing torches");

	for (Wall* _wall : walls)
	{
		_wall->SpawnTorch();
	}
}

void Generator::SpawnPlayer()
{
	loadingText->GetText()->setString("Placing player");

	Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
	EraseElement(spawnablePositions, _position);
	EntityManager::GetInstance().Get("Player")->GetShape()->setPosition(_position);
}

void Generator::SpawnStairs()
{
	loadingText->GetText()->setString("Placing stairs");

	Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
	EraseElement(spawnablePositions, _position);
	stairs.push_back(new Stair(_position, Map::GetInstance().GetCurrentZone()));
}

void Generator::GenerateWalls()
{
	loadingText->GetText()->setString("Generating walls");

	PlaceWallsAroundFloor(floors, 4, true, WT_DIRT);
}

void Generator::SetAllFloorOriginColor()
{
	loadingText->GetText()->setString("Setting floors colors");

	const int _size = (const int)floors.size();
	for (int _index = 0; _index < _size; _index++)
	{
		SetFloorColor(floors[_index], true);
	}
}

void Generator::UpdateDoors()
{
	loadingText->GetText()->setString("Updating doors");

	for (Entity* _entity : others)
	{
		if (Door* _door = dynamic_cast<Door*>(_entity))
		{
			_door->ComputeDirection();
		}
	}
}

void Generator::GetSpawnablePositions()
{
	spawnablePositions.clear();

	loadingText->GetText()->setString("Getting spawnable positions");

	for (Tile* _floor : floors)
	{
		spawnablePositions.push_back(_floor->GetPosition());
	}

	if (shop)
	{
		for (Tile* _floor : shop->GetFloor())
		{
			EraseElement(spawnablePositions, _floor->GetPosition());
		}
	}
}

void Generator::GenerateDiamond(const int _diamondOnFloor, int _diamondInWall)
{
	loadingText->GetText()->setString("Generating diamonds");

	for (int _i = 0; _i < _diamondOnFloor; _i++)
	{
		Vector2f _position = GetRandomElementInVector(spawnablePositions);
		others.push_back(new Pickable(PIT_DIAMOND, STRING_ID("Diamond"), _position));
		EraseElement(spawnablePositions, _position);
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
	loadingText->GetText()->setString("Spawning enemies");

	vector<function<Entity* (const Vector2f& _position)>> _enemyList =
	{
		[this](const Vector2f& _position) { return new NormalBat(_position); },
		[this](const Vector2f& _position) { return new RedBat(_position); },
		[this](const Vector2f& _position) { return new BlackBat(_position); },
		[this](const Vector2f& _position) { return new GreenSlime(_position); },
		[this](const Vector2f& _position) { return new BlueSlime(_position); },
		[this](const Vector2f& _position) { return new OrangeSlime(_position); },
		[this](const Vector2f& _position) { return new NormalSkeleton(_position); },
	};

	for (int _index = 0; _index < _amountOfEnemies; _index++)
	{
		if (spawnablePositions.empty()) return;
		Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
		EraseElement(spawnablePositions, _position);
		others.push_back(_enemyList[Random(static_cast<int>(_enemyList.size() - 1), 0)](_position));
	}
}

void Generator::SpawnTraps(const int _amount)
{
	loadingText->GetText()->setString("Spawning traps");

	for (int _index = 0; _index < _amount; _index++)
	{
		if (spawnablePositions.empty()) return;

		Vector2f _position = spawnablePositions[Random((int)spawnablePositions.size() - 1, 0)];
		EraseElement(spawnablePositions, _position);
		others.push_back(new Trap(_position));
	}
}

void Generator::AddFloorAt(const Vector2f& _position)
{
	Tile* _floor = new Tile(_position);
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

	for (Tile* _floor : floors)
	{
		_floor->InvertAlpha(isPurple, _hasChain);
	}
}

void Generator::GenUpdate()
{
	if (generationIndex >= 0)
	{
		vector<function<void()>> _functionList = {
			// 1- place shop room
			[&]() { GenerateShopRoom(); },
			// 2- generate rooms
			[&]() {	GenerateRooms(6); },
			// 3- generate paths
			[&]() { GeneratePaths(); },
			// 4- floor colors
			[&]() { SetAllFloorOriginColor(); },
			// 5- generate walls
			[&]() { GenerateWalls(); },
			// 6- get spawnable positions
			[&]() { GetSpawnablePositions(); },
			// 7- generate diamonds
			[&]() { GenerateDiamond(); },
			// 8- generate chests
			[&]() { GenerateChest(2); },
			// 9- spawn player
			[&]() { SpawnPlayer(); },
			// 10- spawn stairs
			[&]() { SpawnStairs(); },
			// 11- spawn enemies
			[&]() { SpawnEnnemy(10); },
			// 12- place shop door
			[&]() { PlaceShopDoor(); },
			// 13- place traps
			[&]() { SpawnTraps(8); },
			// 14- place torches
			[&]() { PlaceTorches(); },
			// 15- update doors
			[&]() { UpdateDoors(); },
			// 16- erase overlappings
			[&]() { EraseOverlappings(); },
			// 17- 3D effect
			[&]() { Enable3DEffect(); }, 
			// 18- end dungeon generation
			[&]() { Map::GetInstance().EndDungeonGeneration(); },
		};
		_functionList[generationIndex]();
		sleep(seconds(0.2f));

		generationIndex++;
		if (generationIndex == _functionList.size())
		{
			MenuManager::GetInstance().ToggleLoading();
			generationIndex = -1;
		}
	}
}

void Generator::Update()
{
	if (!*discoModeEnabled)
	{
		return;
	}

	isPurple = !isPurple;
	if (tempoIndex >= 3)
	{
		tempoIndex = 1;
	}
	UpdateTilesColor();
	tempoIndex++;
}