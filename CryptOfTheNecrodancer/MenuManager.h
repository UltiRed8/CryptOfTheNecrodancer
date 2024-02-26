#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Menu.h"

#include <string>

class MenuManager : public Singleton<MenuManager>, public IManager<string, Menu>
{

public:
	void Open(const string& _id, RenderWindow* _window);
};