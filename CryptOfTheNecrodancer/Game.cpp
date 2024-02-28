#include "Game.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "UIImage.h"
#include "ProgressBar.h"
#include "CameraManager.h"
#include "MovementComponent.h"
#include "UIText.h"

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
	MenuManager::GetInstance().InitMenu(window);
	TimerManager::GetInstance().SetMaxFrameRate(60);
	TimerManager::GetInstance().SetRenderCallback([this]() {
		EntityManager::GetInstance().Update();
		UpdateWindow();
	});
}

void Game::Start()
{
	player = new Player("Player",{0.f,0.f},{});

	Init();

	// TODO temporaire

	Map::GetInstance().Generate(6);
	Map::GetInstance().SpawnEnnemy(25);
	//Map::GetInstance().Load("Assets/Saved/Lobby.txt");

	//TheWighttoRemain.mp3 160
	//Lobby.mp3 130
	//zone4_1.ogg 130

	MusicManager::GetInstance().Play("zone4_1.ogg", 130);

	Camera* _playerCamera = new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f,0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	player->GetShape()->setPosition(Map::GetInstance().GetFirstTilePosition());
}

void Game::Update()
{
	while (window->isOpen())
	{
		TimerManager::GetInstance().Update();
		InputManager::GetInstance().Update(window);
		MenuManager::GetInstance().Update();
		CameraManager::GetInstance().Update(window);
	}
	Stop();
}

void Game::UpdateWindow()
{
	window->setView(*CameraManager::GetInstance().Get("PlayerCamera"));
	window->clear();
	for (Drawable* _drawable : EntityManager::GetInstance().GetDrawables())
	{
		window->draw(*_drawable);
	}
	window->setView(window->getDefaultView());
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