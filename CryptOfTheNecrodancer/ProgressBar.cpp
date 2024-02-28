#include "ProgressBar.h"
#include "TextureManager.h"

ProgressBar::ProgressBar(const ProgressType& _type, const Vector2f& _position, const Vector2f& _size,
	const string& _emptyPath, const string& _fullPath, float* _currentValue, const float _maxValue) : UIElement(_position)
{
    type = _type;

    background = new RectangleShape(_size);
    foreground = new RectangleShape(_size);

    background->setPosition(_position);
    
    UpdateOriginAndPosition();

    maxValue = _maxValue;
    currentValue = _currentValue;

    InitTextures(_fullPath, _emptyPath);
}

ProgressBar::~ProgressBar()
{
    delete foreground;
    delete background;
}

void ProgressBar::UpdateOriginAndPosition()
{
    const Vector2f& _barPosition = background->getPosition();
    const Vector2f& _size = background->getGlobalBounds().getSize();

    if (type == PT_LEFT)
    {
        foreground->setOrigin(0.0f, 0.0f);
        foreground->setPosition(_barPosition);
    }
    else if (type == PT_TOP)
    {
        foreground->setOrigin(0.0f, 0.0f);
        foreground->setPosition(_barPosition);
    }
    else if (type == PT_CENTER)
    {
        foreground->setOrigin(_size / 2.0f);
        foreground->setPosition(_barPosition + Vector2f(_size.x / 2.0f, _size.y / 2.0f));
    }
    else if (type == PT_RIGHT)
    {
        foreground->setOrigin(_size.x, 0.0f);
        foreground->setPosition(_barPosition + Vector2f(_size.x, 0.0f));
    }
    else if (type == PT_BOTTOM)
    {
        foreground->setOrigin(0.0f, _size.y);
        foreground->setPosition(_barPosition + Vector2f(0.0f, _size.y));
    }
}

IntRect ProgressBar::MakeRect(const float _percent)
{
    const Vector2f& _textureSize = Vector2f(background->getTexture()->getSize());
    FloatRect _rect;

    if (type == PT_CENTER)
    {
        _rect.left = (_textureSize.x - _percent * _textureSize.x) / 2.0f;
        _rect.top = (_textureSize.x - _percent * _textureSize.x) / 2.0f;
        _rect.width = _percent * _textureSize.x;
        _rect.height = _percent * _textureSize.y;
    }
    else if (type == PT_LEFT || type == PT_TOP)
    {
        _rect.top = 0.0f;
        _rect.left = 0.0f;
        _rect.height = _textureSize.y * (type == PT_TOP ? _percent : 1.0f);
        _rect.width = _textureSize.x * (type == PT_LEFT ? _percent : 1.0f);
    }
    else if (type == PT_RIGHT)
    {
        _rect.top = 0.0f;
        _rect.height = _textureSize.y;
        _rect.left = _textureSize.x - _percent * _textureSize.x;
        _rect.width = _percent * _textureSize.x;
    }
    else if (type == PT_BOTTOM)
    {
        _rect.top = _textureSize.y - _percent * _textureSize.y;
        _rect.height = _percent * _textureSize.y;
        _rect.left = 0.0f;
        _rect.width = _textureSize.x;
    }
    return IntRect(_rect);
}

void ProgressBar::InitTextures(const string& _full, const string& _empty)
{
    TextureManager::GetInstance().Load(background, _empty);
    TextureManager::GetInstance().Load(foreground, _full);
}

vector<Drawable*> ProgressBar::GetDrawables()
{
    vector<Drawable*> _drawables;
    _drawables.push_back(background);
    _drawables.push_back(foreground);
    return _drawables;
}

void ProgressBar::Update(const Vector2i& _mousePosition)
{
    const float _fillPercent = *currentValue / maxValue;
    const IntRect& _rect = MakeRect(_fillPercent);
    foreground->setTextureRect(_rect);

    Vector2i _directions[] = {
        Vector2i(1, 1),
        Vector2i(1, 0),
        Vector2i(0, 1),
        Vector2i(1, 0),
        Vector2i(0, 1),
    };

    Vector2f _scale = foreground->getScale();
    if (_directions[type].x) _scale.x = _fillPercent;
    if (_directions[type].y) _scale.y = _fillPercent;
    foreground->setScale(_scale);
}