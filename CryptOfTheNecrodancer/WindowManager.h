#pragma once

#include "Singleton.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WindowManager : public Singleton<WindowManager>
{
	RenderWindow* window;
	int currentValue;
	Vector2i baseWindowPosition;

public:
	WindowManager();
	~WindowManager();

public:
	RenderWindow* GetWindow() const 
	{
		return window;
	}

private:
	void CreateWindow();

public:
	void Rename(const string& _newWindowName);
	void Shake(const int _strength);
	void Update();
};