#include "Menu.h"

#include "MenuManager.h"
#include "Action.h"

Menu::Menu(const string& _id, const vector<UIElement*>& _elements, const string& _backgroundPath, const Color& _color) : IManagable(_id)
{
	wantsToCloseMenu = false;
	elements = _elements;
	backgroundShape = new RectangleShape(Vector2f(1200.0f, 800.0f));
	backgroundShape->setFillColor(_color);
	if (!_backgroundPath.empty())
	{
		TextureManager::GetInstance()->Load(backgroundShape, _backgroundPath, false, true);
	}
	else
	{
		backgroundShape->setFillColor(Color::Transparent);
	}
	Register();
}

Menu::~Menu()
{
	for (UIElement* _element : elements)
	{
		delete _element;
	}
	delete backgroundShape;
	delete background;
}

void Menu::Open(RenderWindow* _window)
{
	isOpened = true;
	EnableInputs();
	while (_window->isOpen() && !wantsToCloseMenu)
	{
		if (!InputManager::GetInstance()->Update(_window)) _window->close();
		Update(_window);
		_window->clear();
		for (Drawable* _drawable : GetDrawables())
		{
			_window->draw(*_drawable);
		}
		_window->display();
	}
	DisableInputs();
	isOpened = false;
	wantsToCloseMenu = false;
}

void Menu::Register()
{
	MenuManager::GetInstance()->Add(id, this);
}

void Menu::EnableInputs()
{
	actions = new ActionMap("Menus", {
		ActionData("Click", this, &Menu::ClickAction, { ActionType::MouseButtonPressed, Mouse::Left }),
	});
}

void Menu::DisableInputs()
{
	actions->Destroy();
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
	_drawables.push_back(backgroundShape);
	for (UIElement* _element : elements)
	{
		vector<Drawable*> _elementDrawables = _element->GetDrawables();
		_drawables.insert(_drawables.end(), _elementDrawables.begin(), _elementDrawables.end());
	}
	return _drawables;
}

void Menu::Close()
{
	wantsToCloseMenu = true;
}