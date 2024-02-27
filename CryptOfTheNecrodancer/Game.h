#pragma once

#include "Macro.h"
#include "Map.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bat.h"

using namespace std;
using namespace sf;

class Game
{
	RenderWindow* window;
	Player* player;
	Map* map;
	Bat* bat;
public:
	Game();
	~Game();

private:
	void Init();
	void Start();
	void Update();
	void UpdateWindow();;
	void Stop();

#pragma region Menu

	void InitMenuPause();
	void InitOption();
	void InitClose();
	void CloseMenu();
	void OptionMenu();

#pragma endregion

public:
	void Launch();
};