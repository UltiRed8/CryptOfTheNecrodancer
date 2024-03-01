#include "Menu.h"

#include "MenuManager.h"
#include "Action.h"
#include "Macro.h"

Menu::Menu(const string& _id, const vector<UIElement*>& _elements, const int& _zIndex, const bool& _isRelativeToCamera) : IManagable(_id)
{
	wantsToCloseMenu = false;
	for (UIElement* _element : _elements)
	{
		_element->SetOwner(this);
		_element->Register();
	}
	zIndex = _zIndex;
	Register();
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

bool Menu::ClickAction()
{
	for (UIElement* _element : GetAllValues())
	{
		UIButton* _button;
		if (_button = dynamic_cast<UIButton*>(_element))
		{
			if (_button->IsHover())
			{
				_button->ExecuteCallback();
				return true;
			}
		}
	}

	return false;
}

void Menu::Update(RenderWindow* _window)
{
	const Vector2i& _mousePosition = Mouse::getPosition(*_window);
	for (UIElement* _element : GetAllValues())
	{
		_element->Update(_mousePosition);
	}
}

vector<Drawable*> Menu::GetDrawables()
{
	vector<Drawable*> _drawables;
	for (UIElement* _element : GetAllValues())
	{
		vector<Drawable*> _elementDrawables = _element->GetDrawables();
		_drawables.insert(_drawables.end(), _elementDrawables.begin(), _elementDrawables.end());
	}
	return _drawables;
}
