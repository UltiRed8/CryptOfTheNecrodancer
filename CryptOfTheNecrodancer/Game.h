#pragma once

#include "Macro.h"
#include "Map.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace std;
using namespace sf;

class Game
{
	RenderWindow* window;
	Player* player;
	Map* map;

public:
	Game();
	~Game();

private:
	void Init();
	void Start();
	void Update();
	void UpdateWindow();;
	void Stop();

	void InitInputPause();

public:
	void Launch();
};