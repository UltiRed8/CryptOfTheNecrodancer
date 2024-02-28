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
	Init();

	// TODO temporaire

	Map::GetInstance().Generate(6);
	Map::GetInstance().SpawnEnnemy(25);
	//Map::GetInstance().Load("Assets/Saved/Lobby.txt");

	//TheWighttoRemain.mp3 160
	//Lobby.mp3 130

	MusicManager::GetInstance().Play("TheWighttoRemain.mp3", 160);

	player = new Player("Player",{0.f,0.f},{});

	Menu* _hud = new Menu("HUD", {
		//Rythmed Heart
		new UIImage(Vector2f(SCREEN_WIDTH/2 - 20*2, SCREEN_HEIGHT - 55*2), Vector2f(40.0f, 50.0f) * 2.0f, "RythmHearts0.png"),

		//Multiplier
		new UIText(Vector2f(SCREEN_WIDTH / 2 - 20 * 0.5, SCREEN_HEIGHT - 55 * 0.2), Color::White, "Coin Multiplier: " + to_string(player->GetChainMultiplier()),15,"Assets/Font/Font.ttf"),

		//Coins
		new UIImage(Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 12.8), Vector2f(25.0f, 25.0f) * 2.0f, "Coin.png"),
		new UIText(Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 12.5), Color::White, "x " + to_string(player->GetRessources().GetMoney()),25,"Assets/Font/Font.ttf"),

		//Diamond
		new UIImage(Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 11.6), Vector2f(25.0f, 25.0f) * 2.0f, "Diamond.png"),
		new UIText(Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 11.3), Color::White, "x " + to_string(player->GetRessources().GetDiamonds()),25,"Assets/Font/Font.ttf"),
	});

	_hud->Open();
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