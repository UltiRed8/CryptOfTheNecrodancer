#pragma once
#include "IManagable.h"
#include <SFML/Graphics.hpp>

class Menu;

using namespace sf;
using namespace std;

class UIElement : public IManagable<string>
{
	Vector2f position;
	Menu* owner;

public:
	void SetOwner(Menu* _owner)
	{
		owner = _owner;
	}

public :
	UIElement(const string& _id, const Vector2f& _position);

public:
	virtual void Update(const Vector2i& _mousePosition) = 0;
	virtual vector<Drawable*> GetDrawables() = 0;
	virtual void Register() override;
};