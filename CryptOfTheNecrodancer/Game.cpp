#include "Game.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "UIImage.h"
#include "CameraManager.h"
#include "MovementComponent.h"


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
	InitInputPause();
}

void Game::Start()
{
	Init();

	// TODO temporaire

	Map::GetInstance().Generate(1);

	//TheWighttoRemain.mp3

	MusicManager::GetInstance().Play("Lobby.mp3", 130);

	player = new Player("Player",{0.f,0.f},{});

	Menu* _hud = new Menu("HUD", {
		new UIImage(Vector2f(SCREEN_WIDTH/2 - 40*2, SCREEN_HEIGHT - 55*2), Vector2f(80.0f, 50.0f) * 2.0f, "RythmHearts.png"),
	});

	_hud->Open();
	Camera* _playerCamera = new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f,0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Game::Update()
{
	while (window->isOpen())
	{
		TimerManager::GetInstance().Update();
		InputManager::GetInstance().Update(window);
		CameraManager::GetInstance().Update(window);
		EntityManager::GetInstance().Update();
		UpdateWindow();
		window->setView(*CameraManager::GetInstance().Get("PlayerCamera"));
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

void Game::InitInputPause()
{
	new Menu("GamePause", { new UIImage(Vector2f(0.f,0.f), Vector2f(window->getSize().x, window->getSize().y), "PauseMenu.png")}, 1);
	new ActionMap("GamePaused",
		{ ActionData("Echap", [this]() { 
			Menu* _menu = MenuManager::GetInstance().Get("GamePause");
			_menu->Toggle();
			}, {Event::KeyPressed, Keyboard::Escape}) });
}

void Game::Launch()
{
	Start();
	Update();
}