#pragma once

#include "Singleton.h"

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

class WindowManager : public Singleton<WindowManager>
{
	RenderWindow* window;
	int currentValue;
	Vector2i baseWindowPosition;
	bool* isShakable;
	Shader* shader;
	vector<string> shaderNames;
	int currentShader;
	Vector2f currentPosition;
	Vector2f direction;
	bool fullscreen;

public:
	void ToggleFullscreen()
	{
		CreateWindow(!fullscreen);
	}
	bool IsInFullscreen() const
	{
		return fullscreen;
	}
	Shader* GetShader() const
	{
		return shader;
	}
	RenderWindow* GetWindow() const
	{
		return window;
	}
	void ToggleShakable() const
	{
		*isShakable = !*isShakable;
	}

public:
	WindowManager();
	~WindowManager();

private:
	void Init();
	void InitShaders();

public:
	void CreateWindow(const bool _fullscreen);
	void LoadShader(const int _id);
	void Rename(const string& _newWindowName);
	void Shake(const Vector2f& _direction = Vector2f(0.0f, 0.0f));
	void Update();
};