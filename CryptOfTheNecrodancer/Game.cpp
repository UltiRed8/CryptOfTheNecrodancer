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

// TODO LIST :
// animation component a refaire
// lightning component qui n'est plus un component
// sound manager :'(
// finir le jeu
// plusieurs zones (textures et musiques)
// finir le jeu
// finir le jeu
// la vie du joueur
// les vies des livings au dessus d'eux
// encore finir le jeu
// ajouter des ennemis
// finir encore plus le jeu
// les torches sur les murs
// terminer le jeu
// le faire en .exe
// avoir fini le jeu
// le vendre
// faire un post sur twitter
// finir le jeu (pour de vrai)
// ajouter des features pour encore plus finir le jeu
// les traps
// et le jeu
// les boss
// non
// fin (le jeu est fini)

void Game::Init()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer");
	MenuManager::GetInstance().InitMenu(window);
	TimerManager::GetInstance().SetMaxFrameRate(60);
	TimerManager::GetInstance().SetRenderCallback([this]() {
		//EntityManager::GetInstance().Update();
		LightningManager::GetInstance().Update();
		UpdateWindow();
	});
}

// TODO finir le jeu

void Game::Start()
{
	player = new Player(100.f,100.f,"Player",{0.f,0.f});

	Init();

	// TODO temporaire
	Map::GetInstance().Load("Assets/Saved/Lobby.txt");
	MusicManager::GetInstance().PlayMain("Lobby", 130, false, true);
	Camera* _playerCamera = new Camera("PlayerCamera", CAMERA_PLAYER, Vector2f(0.f,0.f), Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Game::Update()
{
	while (window->isOpen())
	{
		MusicManager::GetInstance().Update();
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