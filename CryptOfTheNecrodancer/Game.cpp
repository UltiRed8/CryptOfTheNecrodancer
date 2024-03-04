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
}

Game::~Game()
{
	delete map;
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

	if (Shader::isAvailable())
	{
		shader = new Shader();;
		if (!shader->loadFromFile("Assets/test.frag", Shader::Fragment))
		{
			cerr << "error" << endl;
			// error...
		}
		//shader->setUniform("wave_phase", 10.0f);
		shader->setUniform("scene", Shader::CurrentTexture);
		shader->setUniform("bloomBlur", Shader::CurrentTexture);
	}

	player = new Player(300.0f,100.f,"Player",{0.f,0.f});

	Init();

<<<<<<< Updated upstream
	// TODO temporaire
	MenuManager::GetInstance().ToggleWarningSeizure();	//Warning Seizure
	Map::GetInstance().Open(Z_LOBBY);

	new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f,0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
=======
	//calibration = new Calibration([&]() {EndCalibration(); });
	EndCalibration();
	//calibration->Start();
	new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f, 0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
>>>>>>> Stashed changes
}

void Game::Update()
{
	while (window->isOpen())
	{
<<<<<<< Updated upstream
=======
		//calibration->Update();
>>>>>>> Stashed changes
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
		window->draw(*_drawable, shader);
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