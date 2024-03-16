#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include "Door.h"
#include "LightningManager.h"
#include "CameraManager.h"
#include "MusicManager.h"

Map::Map()
{
	zoneFileName = "";
	currentZone = Z_ZONE1;
	currentLevel = 0;
	tempoIndex = 1;
	chainToggle = true;
	isPurple = false;
	discoModeEnabled = new bool(true);
	generator = new Generator(discoModeEnabled);
}

Map::~Map()
{
	delete generator;
	delete discoModeEnabled;
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
			currentZone = preparedZone;
			NextFloor();
		}
	}
	else
	{
		if (preparedZone != Z_LOBBY)
		{
			((Player*)EntityManager::GetInstance().Get("Player"))->GetInventory()->Reset();
		}
		currentZone = preparedZone;
		LoadMap();
	}
}

void Map::ClearGenerator()
{
	((Player*)EntityManager::GetInstance().Get("Player"))->UpdateDamageZone();

	delete generator;
	generator = new Generator(discoModeEnabled);
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
		GenerateDungeon();
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
		dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->SetDiamonds(0);
		currentLevel = 1;
		GenerateDungeon();
	}
}

void Map::GenerateDungeon()
{
	ClearGenerator();
	generator->Generate(CreateGenerationSettingsByZoneName());
}

GenerationSettings Map::CreateGenerationSettingsByZoneName()
{
	vector<GenerationSettings> _generationSettings = { GenerationSettings({
			[this](const Vector2f& _position) { return new NormalBat(_position); },
			[this](const Vector2f& _position) { return new GreenSlime(_position); },
			[this](const Vector2f& _position) { return new BlueSlime(_position); },
			[this](const Vector2f& _position) { return new NormalSkeleton(_position); },
			},
			6, // RoomsAmount
			3, // WallsWidth
			1, // Diamonds
			2, // DiamondsInWalls
			2, // Chests
			10, // Enemies
			5, // Traps	
			true), 

			GenerationSettings({
			[this](const Vector2f& _position) { return new NormalBat(_position); },
			[this](const Vector2f& _position) { return new RedBat(_position); },
			[this](const Vector2f& _position) { return new GreenSlime(_position); },
			[this](const Vector2f& _position) { return new BlueSlime(_position); },
			[this](const Vector2f& _position) { return new OrangeSlime(_position); },
			[this](const Vector2f& _position) { return new NormalSkeleton(_position); },
			[this](const Vector2f& _position) { return new YellowSkeleton(_position); },
			[this](const Vector2f& _position) { return new Wraith(_position); },
			},
			8, // RoomsAmount
			3, // WallsWidth
			1, // Diamonds
			2, // DiamondsInWalls
			2, // Chests
			15, // Enemies
			10, // Traps
			true), 
			
			GenerationSettings({
			[this](const Vector2f& _position) { return new RedBat(_position); },
			[this](const Vector2f& _position) { return new BlackBat(_position); },
			[this](const Vector2f& _position) { return new BlueSlime(_position); },
			[this](const Vector2f& _position) { return new OrangeSlime(_position); },
			[this](const Vector2f& _position) { return new YellowSkeleton(_position); },
			[this](const Vector2f& _position) { return new BlackSkeleton(_position); },
			[this](const Vector2f& _position) { return new Wraith(_position); },
			},
			10, // RoomsAmount
			3, // WallsWidth
			3, // Diamonds
			4, // DiamondsInWalls
			4, // Chests
			20, // Enemies
			20, // Traps
			true )
	};

	return _generationSettings[preparedZone -1];
}

void Map::EndDungeonGeneration()
{
	CameraManager::GetInstance().Get("PlayerCamera")->SetCameraToPlayer();
	UpdateLights(2);
	PrepareMusic();
	MusicManager::GetInstance().Play();
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetComponent<LifeComponent>()->SetIsInvulnerable(false);
}

void Map::QuickRestart()
{
	((Player*)EntityManager::GetInstance().Get("Player"))->GetInventory()->Reset();

	UpdateZoneFileName();
	dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->SetDiamonds(0);
	currentLevel = 1;

	vector<string> _zones = {
		"zone1_",
		"zone1_",
		"zone2_",
	};
	zoneFileName = _zones[preparedZone] + to_string(currentLevel);
	GenerateDungeon();
}

void Map::RemoveWall(Wall* _wall)
{
	EraseElement(generator->GetWalls(), _wall);
	_wall->Destroy();
}

void Map::UpdateLights(const int _brightness)
{
	LightningManager& _light = LightningManager::GetInstance();
	_light.ClearAll();
	_light.Construct(generator->GetAllWallsAndFloorPositions(), _brightness);
}

void Map::UpdateZoneFileName()
{
	vector<string> _zones = {
		"zone1_",
		"zone1_",
		"zone2_",
	};

	zoneFileName = _zones[preparedZone] + to_string(currentLevel + 1);
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

	MusicManager::GetInstance().PrepareMusic(zoneFileName, true);
}

void Map::OpenLobby()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetInventory()->Reset();
	_player->ResetLife();
	_player->GetRessources()->SetMoney(0);
	_player->SetIsConfuse(false);
	_player->ResetChainMultiplier();
	UpdateZoneFileName();
	ClearGenerator();
	generator->GenerateLobby();
	MusicManager::GetInstance().Prepare("lobby", false, true);
}

void Map::AddFloorAt(const Vector2f& _position)
{
	generator->AddFloorAt(_position);
}

void Map::AddItem(Item* _item)
{
	generator->AddItem(_item);
}

void Map::Update()
{
	generator->Update();
}

Entity* Map::AddOther(Entity* _entity)
{
	generator->AddOther(_entity);
	return _entity;
}