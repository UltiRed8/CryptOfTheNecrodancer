#include "UIButton.h"
#include "SoundManager.h"

#define PATH_SOUND_CLICK "Assets/Sounds/ButtonClick.mp3"

UIButton::UIButton(const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _textValue, const int _textSize, const string& _fontPath, const function<void()>& _callback, int* _additionalValue)
	: UIText(_position, _hoverColor, _textValue, _textSize, _fontPath, _additionalValue)
{
	Vector2f _shapeSize = text->getGlobalBounds().getSize() + Vector2f(30.0f, 30.0f);
	unhoverColor = _unhoverColor;
	hoverColor = _hoverColor;
	callback = _callback;
}

void UIButton::Update(const Vector2i& _mousePosition)
{
	if (text->getGlobalBounds().contains(Vector2f(_mousePosition)))
	{
		text->setFillColor(hoverColor);
	}
	else
	{
		text->setFillColor(unhoverColor);
	}

	UIText::Update(_mousePosition);
}

vector<Drawable*> UIButton::GetDrawables()
{
	vector<Drawable*> _drawables;
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