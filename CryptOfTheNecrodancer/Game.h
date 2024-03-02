#pragma once
#include "Map.h"
#include <iostream>
#include "Player.h"

using namespace std;

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
	void UpdateWindow();
	void Stop();

public:
	void Launch();
};