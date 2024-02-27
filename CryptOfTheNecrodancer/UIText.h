#pragma once

#include <SFML/Graphics.hpp>

#include "UIElement.h"
#include "FontManager.h"

using namespace sf;
using namespace std;

class UIText : public UIElement
{
	Font font;
	int* value;
	string textValue;
protected:
	Text* text;

public:
	UIText(const Vector2f& _position, const Color& _color, const string& _textValue, const int _textSize, const string& _fontPath, bool _italic = false, int* _additionalValue = nullptr);
	~UIText();

public:
	virtual void Update(const Vector2i& _mousePosition) override;
	virtual vector<Drawable*> GetDrawables() override;
};