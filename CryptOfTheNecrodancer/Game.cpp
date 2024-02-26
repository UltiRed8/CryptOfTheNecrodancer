#include "Game.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "InputManager.h"

Game::Game()
{
	window = nullptr;
	map = nullptr;
}

Game::~Game()
{
	delete window;
	delete map;
}

void Game::Init()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer");
}

void Game::Start()
{
	Init();
	map = new Map(1);
	MusicManager::GetInstance().Play("Lobby.mp3");
	//TheWighttoRemain.mp3
	new Timer("Timer", [this]() { map->Update(); }, seconds(1.f/(130/60.f)), -1);
	player = new Player("ZPlayer",{0.f,0.f},{});
}

void Game::Update()
{
	while (window->isOpen())
	{
		TimerManager::GetInstance().Update();
		InputManager::GetInstance().Update(window);
		EntityManager::GetInstance().Update();
		UpdateWindow();
	}
	Stop();
}

void Game::UpdateWindow()
{
	window->clear();
	const vector<Entity*>& _entities = EntityManager::GetInstance().GetAllValues();
	for (Entity* _entity : _entities)
	{
		window->draw(*_entity->GetShape());
	}
	window->display();
}

void Game::Stop()
{
	cout << "Fin!" << endl;
}

void Game::Launch()
{
	Start();
	Update();
}