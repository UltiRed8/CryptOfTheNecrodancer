#include "RythmIndicator.h"
#include "TimerManager.h"
#include "MusicManager.h"

RythmIndicator::RythmIndicator(const RythmIndicatorDirection& _direction, Menu* _owner, const string& _path, const float _speed) : UIImage(STRING_ID("ARythmIndicator"), GetPositionWithDirection(_direction),{12.f,50.f}, _path)
{
	directionType = _direction;
	SetDirectionWithDirectionType();
	SetOwner(_owner);
	Register();

	opacity = 0;
	shape->setFillColor({ 255,255,255,(Uint8)opacity });
	speed = _speed;
}

RythmIndicator::RythmIndicator(const RythmIndicatorDirection& _direction, Menu* _owner, const string& _path, const float _speed, const float _posX) : UIImage(STRING_ID("ARythmIndicator"), Vector2f(0.0f, 0.0f), {12.f,50.f}, _path)
{
	directionType = _direction;
	SetDirectionWithDirectionType();
	SetOwner(_owner);
	Register();

	Vector2f _position = GetPositionWithDirection(_direction);
	_position.x = _posX - 6.0f;
	shape->setPosition(_position);

	opacity = 0;
	shape->setFillColor({ 255,255,255,(Uint8)opacity });
	speed = _speed;
}

void RythmIndicator::Update(const Vector2i& _mousePosition)
{
	if (!MusicManager::GetInstance().IsRunning()) return;

	const float _deltaTime = TimerManager::GetInstance().GetDeltaTime();
	const Vector2f& _currentPosition = shape->getPosition();
	if (opacity != 255.f)
	{
		opacity += _deltaTime * 0.5f;
		shape->setFillColor({ 255,255,255,(Uint8)(opacity > 255.f ? 255.f : opacity)});
	}

	if (directionType == RID_RIGHT && _currentPosition.x >= SCREEN_WIDTH /2)
	{
		Destroy();
	}

	else if (directionType == RID_LEFT && _currentPosition.x <= SCREEN_WIDTH / 2)
	{
		Destroy();
	}

	shape->move({ speed * direction * _deltaTime});
}