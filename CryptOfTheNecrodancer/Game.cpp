#include "Game.h"

Game::Game()
{
	window = nullptr;
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
	//window->draw();
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