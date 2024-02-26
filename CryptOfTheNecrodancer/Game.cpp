#include "Game.h"
#include "EntityManager.h"

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
}

void Game::Update()
{
	while (window->isOpen())
	{
		UpdateInputs();
		UpdateWindow();
	}
	Stop();
}

void Game::UpdateWindow()
{
	window->clear();
	for (Entity* _entity : EntityManager::GetInstance()->GetAllValues())
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