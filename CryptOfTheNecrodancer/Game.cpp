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

Game::Game()
{
	map = nullptr;
	player = nullptr;
	calibration = nullptr;
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

	/*calibration = new Calibration([&]() {EndCalibration(); });
	calibration->Start();*/
	EndCalibration();
	new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f, 0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Game::Update()
{
	while (window->isOpen())
	{
		//calibration->Update();
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

void Game::EndCalibration()
{
	MenuManager::GetInstance().ToggleWarningSeizure();	//Warning Seizure
	Map::GetInstance().Open(Z_LOBBY);
}

void Game::Launch()
{
	Start();
	Update();
}