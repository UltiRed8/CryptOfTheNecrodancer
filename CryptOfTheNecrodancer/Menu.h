#pragma once

#include <SFML/Graphics.hpp>

#include "IManagable.h"
#include "UIButton.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "UIIMage.h"

using namespace sf;
using namespace std;

class Menu : public IManagable<string>, public IManager<string, UIElement>
{
	bool wantsToCloseMenu;
	bool isOpened;	
	int zIndex;

public:
	int GetZIndex() const
	{
		return zIndex;
	}
	vector<UIElement*> GetElements() const
	{
		return GetAllValues();
	}
	bool IsOpened() const
	{
		return isOpened;
	}

public:
	Menu(const string& _id, const vector<UIElement*>& _elements, const int& _zIndex = 0, const bool& _isRelativeToCamera = false);

private:
	virtual void Register() override;

public:
	bool ClickAction();
	void Update(RenderWindow* _window);
	void Open();
	void Close();
	void Toggle();
	vector<Drawable*> GetDrawables();
};