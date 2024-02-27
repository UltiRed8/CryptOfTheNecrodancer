#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "UIElement.h"
#include "FontManager.h"

using namespace sf;
using namespace std;

class UIButton : public UIElement
{
	FloatRect box;
	Color unhoverColor;
	Color hoverColor;
	function<void()> callback;
	vector<UIElement*> elements;
	bool isHover;

public:
	bool IsHover() const
	{
		return isHover;
	}

public:
	// Text
	UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _textValue, const int _textSize, const string& _fontPath, const function<void()>& _callback, int* _additionalValue = nullptr);
	// Image
	UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _path, const Vector2f& _imageSize, const function<void()>& _callback);
	// Custom
	UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const vector<UIElement*>& _elements, const function<void()>& _callback, const FloatRect& _box);
	~UIButton();

public:
	virtual void Update(const Vector2i& _mousePosition) override;
	virtual vector<Drawable*> GetDrawables() override;
	void ExecuteCallback();
};