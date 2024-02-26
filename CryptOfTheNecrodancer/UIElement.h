#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class UIElement
{
	Vector2f position;

public:
	UIElement(const Vector2f& _position);

public:
	virtual void Update(const Vector2i& _mousePosition) = 0;
	virtual vector<Drawable*> GetDrawables() = 0;
};