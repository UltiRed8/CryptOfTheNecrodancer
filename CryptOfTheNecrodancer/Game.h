#pragma once
#include "Map.h"
#include <iostream>
#include "Player.h"
#include "Calibration.h"

using namespace std;

class Game
{
	RenderWindow* window;
	Player* player;
	Map* map;
	Calibration* calibration;
	float delta;

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