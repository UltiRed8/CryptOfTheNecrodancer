#include "Animation.h"
#include "AnimationComponent.h"
#include "Timer.h"
#include"EntityManager.h"
#include"Macro.h"

Animation::Animation(const string& _name, AnimationComponent* _owner, Sprite* _sprite, const AnimationData& _data) : IManagable(STRING_ID(_name))
{
	owner = _owner;
	sprite = _sprite;
	data = _data;
	Register();
	const Vector2i& _position = Vector2i(static_cast<const int>(data.start.x), static_cast<const int>(data.start.y));
	const Vector2i& _size = Vector2i(static_cast<const int>(data.size.x), static_cast<const int>(data.size.y));
	const IntRect& _rect = IntRect(_position, _size);
	sprite->setTextureRect(_rect);
}


void Animation::Register()
{
	owner->Add(id, this);
}

void Animation::SetNext()
{
	if (!CanNext())
	{
		if (!data.canLoop)
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
	sprite->setTextureRect(_rect);
	Update();
}

Vector2i Animation::GetNewStart()
{
	int _x = static_cast<int>(data.start.x);
	int _y = static_cast<int>(data.start.y);

	switch (data.readDirection)
	{
	case READ_DOWN:
		_y += static_cast<int>(currentIndex * data.size.y);
		break;

	case READ_RIGHT:
		_x += static_cast<int>(currentIndex * data.size.x);
		break;

	default:
		break;
	}

	return Vector2i(_x, _y);
}

void Animation::Start()
{
	if (!timer)
	{

	timer = new Timer("Animation" + data.name, this, &Animation::SetNext, seconds(data.timeBetween), -1,false);
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

void Animation::Stop()
{
	if (!timer) return;
	timer->Pause();
}

void Animation::Update()
{
	Vector2f _spriteSize = sprite->getLocalBounds().getSize();
	Vector2f _shapeSize = owner->GetOwner()->GetShape()->getLocalBounds().getSize();
	Vector2f _scale = Vector2f(_shapeSize.x / _spriteSize.x, _shapeSize.y / _spriteSize.y);
	sprite->setScale(_scale);
	sprite->setPosition(owner->GetOwner() ->GetPosition() + Vector2f(_shapeSize.x* _scale.x, _shapeSize.y/_scale.y));
}