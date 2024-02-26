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
	Texture* background;
	RectangleShape* backgroundShape;
	bool wantsToCloseMenu;
	bool isOpened;
	ActionMap* actions;

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
	Menu(const string& _id, const vector<UIElement*>& _elements, const string& _backgroundPath = "", const Color& _color = Color(255, 255, 255));
	~Menu();

private:
	virtual void Register() override;
	void EnableInputs();
	void DisableInputs();
	void ClickAction();

public:
	void Update(RenderWindow* _window);
	void Open(RenderWindow* _window);
	vector<Drawable*> GetDrawables();
	void Close();
};