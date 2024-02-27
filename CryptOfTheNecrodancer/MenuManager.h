#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Menu.h"

#include <string>

class MenuManager : public Singleton<MenuManager>, public IManager<string, Menu>
{
public:
	vector<Drawable*> GetDrawables();
	
public :
	void ClickAction();
	void Update(RenderWindow* _window);
};