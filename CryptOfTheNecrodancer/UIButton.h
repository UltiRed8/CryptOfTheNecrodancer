#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "UIText.h"
#include "FontManager.h"

using namespace sf;
using namespace std;

class UIButton : public UIText
{
	Color unhoverColor;
	Color hoverColor;
	RectangleShape* shape;
	function<void()> callback;

public:
	bool IsHover() const
	{
		return shape->getFillColor() == hoverColor;
	}

public:
	UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const Color& _textColor, const string& _textValue, const int _textSize, const string& _fontPath, const function<void()>& _callback, int* _additionalValue = nullptr);
	~UIButton();

public:
	virtual void Update(const Vector2i& _mousePosition) override;
	virtual vector<Drawable*> GetDrawables() override;
	void ExecuteCallback();
};