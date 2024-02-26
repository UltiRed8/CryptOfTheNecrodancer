#include "Game.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "UIImage.h"

Game::Game()
{
	window = nullptr;
	map = nullptr;
	player = nullptr;
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

	// TODO temporaire

	map = new Map(1);

	MusicManager::GetInstance().Play("Lobby.mp3");

	//TheWighttoRemain.mp3

	new Timer("Timer", [this]() { map->Update(); }, seconds(1.f/(130/60.f)), -1);

	player = new Player("Player",{0.f,0.f},{});

	Menu* _hud = new Menu("HUD", {
		new UIImage(Vector2f(SCREEN_WIDTH/2 - 40*2, SCREEN_HEIGHT - 55*2), Vector2f(80.0f, 50.0f) * 2.0f, "RythmHearts.png"),
	});

	_hud->Open();
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
	for (Drawable* _drawable : EntityManager::GetInstance().GetDrawables())
	{
		window->draw(*_drawable);
	}
	for (Drawable* _drawable : MenuManager::GetInstance().GetDrawables())
	{
		window->draw(*_drawable);
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