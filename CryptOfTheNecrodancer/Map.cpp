#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include "Door.h"
#include "LightningManager.h"
#include "CameraManager.h"
#include "Hephaestus.h"

#define PATH_SHOP_TILE "Dungeons/ShopTile.png"
#define PATH_UPGRADE_TILE "Dungeons/UpgradeTile.png"
#define PATH_FLOOR "Dungeons/" + GetZoneFileName() + "/floor.png"

#define C_BROWN Color(135, 79, 2, 255)
#define C_LIGHT_BROWN Color(135, 79, 2, 200)

Map::Map()
{
	zoneFileName = "";
	currentZone = Z_ZONE1;
	currentLevel = 0;
	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;
	generator = new Generator();
}

Map::~Map()
{
	delete generator;
}

void Map::Prepare(const Zone& _zoneToOpen)
{
	preparedZone = _zoneToOpen;
}

void Map::Open(const Zone& _zoneToOpen)
{
	Prepare(_zoneToOpen);
	OpenPrepared();
}

void Map::OpenPrepared()
{
	UpdateZoneFileName();
	if (preparedZone == currentZone)
	{
		if (currentZone != Z_LOBBY)
		{
			NextFloor();
		}
	}
	else
	{
		LoadMap();
		currentZone = preparedZone;
	}
}

void Map::ClearGenerator()
{
	delete generator;
	generator = new Generator();
}

void Map::NextFloor()
{
	currentLevel++;
	if (currentLevel > 3)
	{
		Open(Z_LOBBY);
		return;
	}
	else
	{
		// Regenerate with dungeon class
		cout << "next floor" << endl;
		ClearGenerator();
		generator->Generate(6, 25);
	}
}

void Map::LoadMap()
{
	if (preparedZone == Z_LOBBY)
	{
		currentLevel = 0;
		OpenLobby();
	}
	else
	{
		currentLevel = 1;
		// Generate with dungeon class
		cout << "open dungeon" << endl;
		ClearGenerator();
		generator->Generate(6, 25);
	}
}

void Map::UpdateZoneFileName()
{
	vector<string> _zones = {
		"zone1_", // Lobby has the same textures as Zone 1
		"zone1_",
		"zone2_",
	};

	zoneFileName = _zones[preparedZone] + to_string(preparedZone);
}

void Map::PrepareMusic()
{
	map<string, int> _bpmList = {
		{ "zone1_1", 115 },
		{ "zone1_2", 130 },
		{ "zone1_3", 140 },

		{ "zone2_1", 130 },
		{ "zone2_2", 140 },
		{ "zone2_3", 150 },

		{ "zone3_1", 135 },
		{ "zone3_2", 145 },
		{ "zone3_3", 155 },

		{ "zone4_1", 130 },
		{ "zone4_2", 145 },
		{ "zone4_3", 160 },

		{ "zone5_1", 130 },
		{ "zone5_2", 140 },
		{ "zone5_3", 155 },
	};

	MusicManager::GetInstance().PrepareMain(zoneFileName, _bpmList[zoneFileName], true);
}

//TODO mettre les methode pour changer les musique en fonction de la zone

void Map::OpenLobby()
{
	UpdateZoneFileName();
	ClearGenerator();
	generator->GenerateLobby();
	MusicManager::GetInstance().PrepareMain("Lobby", 130, false, true);
	MusicManager::GetInstance().Play();
}

void Map::AddFloorAt(const Vector2f& _position)
{
	generator->AddFloorAt(_position,GetZoneFileName());
}

void Map::Update()
{
	generator->Update();
}

void Map::AddOther(Entity* _entity)
{
	generator->AddOther(_entity);
}
