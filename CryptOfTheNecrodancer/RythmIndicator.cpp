#include "RythmIndicator.h"
#include "TimerManager.h"

RythmIndicator::RythmIndicator(const RythmIndicatorDirection& _direction,Menu* _owner, const string& _path) : UIImage(STRING_ID("ARythmIndicator"), GetPositionWithDirection(_direction),{12.f,50.f}, _path)
{
	directionType = _direction;
	SetDirectionWithDirectionType();
	SetOwner(_owner);
	Register();

	shape->setFillColor({ 255,255,255,0 });
	opacity = 0;
	speed = { 0.5f,0.5f };

	isRunning = true;
}

void RythmIndicator::Update(const Vector2i& _mousePosition)
{
	if (!isRunning) return;

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

void RythmIndicator::Resume()
{
	isRunning = true;
}

void RythmIndicator::Pause()
{
	isRunning = false;
}