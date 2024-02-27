#include "Menu.h"

#include "MenuManager.h"
#include "Action.h"

Menu::Menu(const string& _id, const vector<UIElement*>& _elements, const int& _zIndex) : IManagable(_id)
{
	wantsToCloseMenu = false;
	elements = _elements;
	zIndex = _zIndex;
	Register();
}

Menu::~Menu()
{
	for (UIElement* _element : elements)
	{
		delete _element;
	}
}

void Menu::Open()
{
	isOpened = true;
}

void Menu::Close()
{
	isOpened = false;
}

void Menu::Toggle()
{
	isOpened = !isOpened;
}

void Menu::Register()
{
	MenuManager::GetInstance().Add(id, this);
}

void Menu::ClickAction()
{
	for (UIElement* _element : elements)
	{
		UIButton* _button;
		if (_button = dynamic_cast<UIButton*>(_element))
		{
			if (_button->IsHover())
			{
				_button->ExecuteCallback();
			}
		}
	}
}

void Menu::Update(RenderWindow* _window)
{
	const Vector2i& _mousePosition = Mouse::getPosition(*_window);
	for (UIElement* _element : elements)
	{
		_element->Update(_mousePosition);
	}
}

vector<Drawable*> Menu::GetDrawables()
{
	vector<Drawable*> _drawables;
	for (UIElement* _element : elements)
	{
		vector<Drawable*> _elementDrawables = _element->GetDrawables();
		_drawables.insert(_drawables.end(), _elementDrawables.begin(), _elementDrawables.end());
	}
	return _drawables;
}