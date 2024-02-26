#pragma once

#include <SFML/Graphics.hpp>

#include "IManagable.h"
#include "UIButton.h"
#include "InputManager.h"
#include "TextureManager.h"

using namespace sf;
using namespace std;

class Menu : public IManagable<string>
{
	vector<UIElement*> elements;
	bool wantsToCloseMenu;
	bool isOpened;

public:
	vector<UIElement*> GetElements() const
	{
		return elements;
	}
	bool IsOpened() const
	{
		return isOpened;
	}

public:
	Menu(const string& _id, const vector<UIElement*>& _elements);
	~Menu();

private:
	virtual void Register() override;
	void ClickAction();

public:
	void Update(RenderWindow* _window);
	void Open();
	void Close();
	vector<Drawable*> GetDrawables();
};