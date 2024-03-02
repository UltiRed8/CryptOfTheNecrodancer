#pragma once

#include "Singleton.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class WindowManager : public Singleton<WindowManager>
{
	RenderWindow* window;

public:
	WindowManager();
	~WindowManager();

public:
	RenderWindow* GetWindow() const 
	{
		return window;
	}

public:
	void CreateWindow();
	void Rename(const string& _newWindowName);
	void Shake(const int _strength);
};