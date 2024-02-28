#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
#include "SoundManager.h"

// Text
UIButton::UIButton(const string& _id, const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _textValue, const int _textSize, const string& _fontPath, const string& _soundPath, const function<void()>& _callback, int* _additionalValue) : UIElement(_id, _position)
{
	unhoverColor = _unhoverColor;
	hoverColor = _hoverColor;
	callback = _callback;
	UIText* _text = new UIText(_id, _position, _unhoverColor, _textValue, _textSize, _fontPath, false, _additionalValue);
	elements.push_back(_text);
	box = _text->GetText()->getGlobalBounds();
	soundPath = _soundPath;
}

// Image
UIButton::UIButton(const string& _id, const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _path, const Vector2f& _imageSize, const string& _soundPath, const function<void()>& _callback) : UIElement(_id, _position)
{
	unhoverColor = _unhoverColor;
	hoverColor = _hoverColor;
	callback = _callback;
	UIImage* _image = new UIImage(_id, _position, _imageSize, _path);
	elements.push_back(_image);
	box = _image->GetShape()->getGlobalBounds();
	soundPath = _soundPath;
}

// Custom
UIButton::UIButton(const string& _id, const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const vector<UIElement*>& _elements, const string& _soundPath, const function<void()>& _callback, const FloatRect& _box) : UIElement(_id, _position)
{
	unhoverColor = _unhoverColor;
	hoverColor = _hoverColor;
	callback = _callback;
	elements = _elements;
	box = _box;
	soundPath = _soundPath;
}

UIButton::~UIButton()
{
	for (UIElement* _element : elements)
	{
		delete _element;
	}
}

void UIButton::Update(const Vector2i& _mousePosition)
{
	isHover = box.contains(Vector2f(_mousePosition));
	for (UIElement* _element : elements)
	{
		if (UIText* _textElement = dynamic_cast<UIText*>(_element))
		{
			_textElement->GetText()->setFillColor(isHover ? hoverColor : unhoverColor);
			_textElement->Update(_mousePosition);
		}
		else if (UIImage* _imageElement = dynamic_cast<UIImage*>(_element))
		{
			_imageElement->GetShape()->setFillColor(isHover ? hoverColor : unhoverColor);
		}
	}
}

vector<Drawable*> UIButton::GetDrawables()
{
	vector<Drawable*> _drawables;
	for (UIElement* _element : elements)
	{
		if (UIImage* _imageElement = dynamic_cast<UIImage*>(_element))
		{
			_drawables.push_back(_imageElement->GetShape());
		}
	}
	for (UIElement* _element : elements)
	{
		if (UIText* _textElement = dynamic_cast<UIText*>(_element))
		{
			_drawables.push_back(_textElement->GetText());
		}
	}
	return _drawables;
}

void UIButton::ExecuteCallback()
{
	SoundManager::GetInstance().Play(soundPath);
	if (callback)
	{
		callback();
	}
}