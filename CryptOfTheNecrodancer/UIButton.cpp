#include "UIButton.h"
#include "SoundManager.h"

#define PATH_SOUND_CLICK "Assets/Sounds/ButtonClick.wav"

UIButton::UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const Color& _textColor, const string& _textValue, const int _textSize, const string& _fontPath, const function<void()>& _callback, int* _additionalValue)
	: UIText(_position, _textColor, _textValue, _textSize, _fontPath, _additionalValue)
{
	;
	Vector2f _shapeSize = text->getGlobalBounds().getSize() + Vector2f(30.0f, 30.0f);
	shape = new RectangleShape(_shapeSize);
	shape->setOrigin(_shapeSize / 2.0f - Vector2f(0.0f, _shapeSize.y / 6.0f ));
	shape->setPosition(text->getPosition());
	shape->setFillColor(_unhoverColor);
	shape->setOutlineThickness(1.0f);
	shape->setOutlineColor(Color::Black);
	unhoverColor = _unhoverColor;
	hoverColor = _hoverColor;
	callback = _callback;
}

UIButton::~UIButton()
{
	delete shape;
}

void UIButton::Update(const Vector2i& _mousePosition)
{
	if (shape->getGlobalBounds().contains(Vector2f(_mousePosition)))
	{
		shape->setFillColor(hoverColor);
	}
	else
	{
		shape->setFillColor(unhoverColor);
	}
	UIText::Update(_mousePosition);
}

vector<Drawable*> UIButton::GetDrawables()
{
	vector<Drawable*> _drawables;
	_drawables.push_back(shape);
	_drawables.push_back(text);
    return _drawables;
}

void UIButton::ExecuteCallback()
{
	SoundManager::GetInstance().Play(PATH_SOUND_CLICK);
	if (callback)
	{
		callback();
	}
}