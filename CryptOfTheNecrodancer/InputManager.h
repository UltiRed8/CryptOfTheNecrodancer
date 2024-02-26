#pragma once

#include "IManager.h"
#include "IManagable.h"
#include "Singleton.h"
#include "ActionMap.h"

#include <vector>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class InputManager : public Singleton<InputManager>, public IManager<string, ActionMap>
{
	Vector2f mousePosition;

public:
	Vector2f GetMousePosition() const
	{
		return mousePosition;
	}

public:
	bool Update(RenderWindow* _window);
	void ClickOnEntities();

private:
	void UpdateInputs(const Event& _event);
};