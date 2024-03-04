#include "Animation.h"
#include "AnimationComponent.h"
#include "Macro.h"

Animation::Animation(const string& _name, AnimationComponent* _owner, Shape* _shape, const AnimationData& _data) : IManagable(STRING_ID(_name))
{
	owner = _owner;
	shape = _shape;
	data = _data;
	Register();

	const int _spritesPerLine = (const int)shape->getTexture()->getSize().x / (int)data.size.x;

	int _posX = (data.start % _spritesPerLine) * (int)data.size.x;
	int _posY = (int)(floor(data.start / _spritesPerLine) * (int)data.size.y);

	data.startPosition = Vector2i(_posX, _posY);

	const Vector2i& _size = Vector2i(static_cast<const int>(data.size.x), static_cast<const int>(data.size.y));
	const IntRect& _rect = IntRect(data.startPosition, _size);
	shape->setTextureRect(_rect);
}

void Animation::Register()
{
	owner->Add(id, this);
}

void Animation::SetNext()
{
	if (!CanNext())
	{
		if (!data.shouldLoop)
		{
			Stop();
			return;
		}

		Reset();
	}

	currentIndex++;

	const Vector2i& _start = GetNewStart();
	const int _sizeX = static_cast<int>(data.size.x);
	const int _sizeY = static_cast<int>(data.size.y);
	const IntRect& _rect = IntRect(_start.x, _start.y, _sizeX, _sizeY);
	shape->setTextureRect(_rect);
}

Vector2i Animation::GetNewStart()
{
	int _x = static_cast<int>(data.startPosition.x);
	const int _y = static_cast<int>(data.startPosition.y);

	_x += static_cast<int>(currentIndex * data.size.x);

	return Vector2i(_x, _y);
}

void Animation::Start()
{
	if (!timer)
	{
		timer = new Timer(STRING_ID("Animation" + data.name), this, &Animation::SetNext, seconds(data.timeBetween), -1,false);
	}
	else
	{
		timer->Resume();
	}
}

void Animation::Reset()
{
	currentIndex = -1;
}

void Animation::Replay()
{
	Reset();
	Start();
}

void Animation::Stop()
{
	if (!timer) return;
	timer->Pause();
}