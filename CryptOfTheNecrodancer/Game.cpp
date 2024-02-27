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
	InitMenuPause();
	InitOption();
	InitClose();
}

void Game::Start()
{
	Init();

	// TODO temporaire

	Map::GetInstance().Generate(3);

	//TheWighttoRemain.mp3

	MusicManager::GetInstance().Play("Lobby.mp3", 130);

	player = new Player("Player",{0.f,0.f},{});

	Menu* _hud = new Menu("HUD", {
		new UIImage(Vector2f(SCREEN_WIDTH/2 - 40*2, SCREEN_HEIGHT - 55*2), Vector2f(40.0f, 50.0f) * 2.0f, "RythmHearts0.png"),
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
		CameraManager::GetInstance().Update(window);
		EntityManager::GetInstance().Update();
		MenuManager::GetInstance().Update(window);

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

void Game::InitMenuPause()
{
	new ActionMap("GamePaused",
		{ ActionData("Escape", [this]()
			{ Menu* _menu = MenuManager::GetInstance().Get("GamePause");
			_menu->Toggle(); },
			{Event::KeyPressed, Keyboard::Escape}),
		ActionData("Select", [this]()
			{ MenuManager::GetInstance().ClickAction(); },
			{Event::MouseButtonPressed, Mouse::Left}) });

	function<void()> _callbackContinue = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); };
	function<void()> _callbackRestart = [this]() {  };
	function<void()> _callbackOptions = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); OptionMenu(); };
	function<void()> _callbackLobby = [this]() {  };
	function<void()> _callbackEchap = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); CloseMenu(); };

	new Menu("GamePause", { new UIImage(Vector2f(0.f,0.f), Vector2f(window->getSize().x, window->getSize().y), "PauseMenu.png"),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 5),Color::White, Color::Cyan, "Continue Game", 50, "Assets/Font/Font.ttf", _callbackContinue),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 3.5), Color::White, Color::Cyan, "Quick Restart", 50, "Assets/Font/Font.ttf", _callbackRestart),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 2.7), Color::White, Color::Cyan, "Options", 50, "Assets/Font/Font.ttf", _callbackOptions),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 2.2), Color::White, Color::Cyan, "Return to Lobby", 50, "Assets/Font/Font.ttf", _callbackLobby),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 1.3), Color::White, Color::Cyan, "Exit Game", 50, "Assets/Font/Font.ttf", _callbackEchap) }, 1);
	
	
}

void Game::InitOption()
{
	function<void()> _close = [this]() { OptionMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("Options", { new UIImage(Vector2f(0.f,0.f), Vector2f(window->getSize().x, window->getSize().y), "OptionsMenu.png"),
		new ProgressBar(PT_LEFT, Vector2f(window->getSize().x / 2.9, window->getSize().y / 1.6), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png"),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 1.3), Color::White, Color::Cyan, "Exit Options", 50, "Assets/Font/Font.ttf", _close) }, 2);

}

void Game::InitClose()
{
	function<void()> _close = [this]() { window->close(); };
	function<void()> _return = [this]() { CloseMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("AreYouSure", { new UIImage(Vector2f(0.f,0.f), Vector2f(window->getSize().x, window->getSize().y), "AreYouSure.png"),
		new UIText(Vector2f(window->getSize().x / 2, window->getSize().y / 3.3), Color::White, "Are you sure you want to leave?",35,"Assets/Font/Font.ttf"),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 1.8), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", _return),
		new UIButton(Vector2f(window->getSize().x / 2, window->getSize().y / 1.6), Color::White, Color::Cyan, "Yes, quit the game", 35, "Assets/Font/Font.ttf", _close) }, 2);
}

void Game::CloseMenu()
{
	MenuManager::GetInstance().Get("AreYouSure")->Toggle();
}

void Game::OptionMenu()
{
	MenuManager::GetInstance().Get("Options")->Toggle();
}

void Game::Launch()
{
	Start();
	Update();
}