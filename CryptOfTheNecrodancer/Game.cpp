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
#include "LightningManager.h"
#include "WindowManager.h"
#include <SFML/Graphics.hpp>

Game::Game()
{
	window = nullptr;
	map = nullptr;
	player = nullptr;
	calibration = nullptr;
	delta = 0.0f;
}

Game::~Game()
{
	delete map;
	delete calibration;
}

void Game::Init()
{
	window = WindowManager::GetInstance().GetWindow();
	MenuManager::GetInstance().InitMenu();
	TimerManager::GetInstance().SetMaxFrameRate(60);
	TimerManager::GetInstance().SetRenderCallback([this]() {
		UpdateWindow();
	});
}

void Game::Start()
{
	player = new Player(300.0f,100.f,"Player",{0.f,0.f});

	Init();

	MenuManager::GetInstance().ToggleWarningSeizure();
	Map::GetInstance().Open(Z_LOBBY);

	new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f, 0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	new Camera("MiniMapCamera", CAMERA_MINIMAP, Vector2f(0,0), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Game::Update()
{
	while (window->isOpen())
	{
		delta += TimerManager::GetInstance().GetDeltaTime();
		MusicManager::GetInstance().Update();
		TimerManager::GetInstance().Update();
		InputManager::GetInstance().Update();
		MenuManager::GetInstance().Update();
		CameraManager::GetInstance().Update();
		WindowManager::GetInstance().Update();
		Map::GetInstance().GetGenerator()->GenUpdate();
	}
	Stop();
}

void Game::UpdateWindow()
{
	Shader* _activeShader = WindowManager::GetInstance().GetShader();

	_activeShader->setUniform("u_time", delta / 1000);
	window->clear();
	window->setView(*CameraManager::GetInstance().Get("PlayerCamera"));
	const vector<Drawable*>& _entities = EntityManager::GetInstance().GetDrawables();
	for (Drawable* _drawable : _entities)
	{
		window->draw(*_drawable, _activeShader);
	}
	window->setView(*CameraManager::GetInstance().Get("MiniMapCamera"));
	for (Drawable* _drawable : _entities)
	{
		window->draw(*_drawable, _activeShader);
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