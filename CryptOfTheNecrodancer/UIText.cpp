#include "UIText.h"

UIText::UIText(const Vector2f& _position, const Color& _color, const string& _textValue, const int _textSize, const string& _fontPath, bool _italic, int* _additionalValue) : UIElement(_position)
{
	value = _additionalValue;
	text = new Text();
	FontManager::GetInstance().Load(text, _fontPath, true);
	text->setFillColor(_color);
	text->setString(_textValue + (_additionalValue != nullptr ? to_string(*value) : ""));
	text->setCharacterSize(_textSize);
	Vector2f _textNewOrigin = text->getGlobalBounds().getSize();
	text->setOrigin(_textNewOrigin / 2.0f);
	text->setPosition(_position);
	textValue = _textValue;

	if (_italic)
	{
		text->setStyle(text->Italic);
	}
}

UIText::~UIText()
{
	delete text;
}

void UIText::Update(const Vector2i& _mousePosition)
{
	if (value)
	{
		text->setString(textValue + to_string(*value));
	}
}

vector<Drawable*> UIText::GetDrawables()
{
	vector<Drawable*> _drawables;
	_drawables.push_back(text);
	return _drawables;
}
