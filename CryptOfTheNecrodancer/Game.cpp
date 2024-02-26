#include "Game.h"
#include "EntityManager.h"
#include "TimerManager.h"

Game::Game()
{
	window = nullptr;
	map = nullptr;
}

Game::~Game()
{
	delete window;
}

void Game::Init()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer");
}

void Game::Start()
{
	Init();
	map = new Map(1);

	new Timer("Timer", [this]() { map->Update(); }, seconds(1.f), -1);
}

void Game::Update()
{
	while (window->isOpen())
	{
		UpdateInputs();
		TimerManager::GetInstance().Update();

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

void Game::UpdateInputs()
{
	Event _event;
	while (window->pollEvent(_event))
	{
		if (_event.type == Event::Closed)
		{
			window->close();
		}
	}
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