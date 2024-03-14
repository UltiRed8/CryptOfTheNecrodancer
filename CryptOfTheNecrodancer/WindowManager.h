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

public:
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
	void CreateWindow();
	void Init();
	void InitShaders();

public:
	void LoadShader(const int _id);
	void Rename(const string& _newWindowName);
	void Shake(const int _strength);
	void Update();
};