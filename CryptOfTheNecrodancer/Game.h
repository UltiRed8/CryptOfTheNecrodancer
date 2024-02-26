#pragma once

#include "Macro.h"

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Game
{
	RenderWindow* window;

public:
	Game();
	~Game();

private:
	void Init();
	void Start();
	void Update();
	void UpdateWindow();
	void UpdateInputs();
	void Stop();

public:
	void Launch();
};