#include "Timer.h"
#include "TimerManager.h"

Timer::Timer(const string& _id, const function<void()>& _callback, const Time& _duration, const int _repeat, const bool _autoDestroy, const bool _register) : IManagable(_id)
{
	callback = _callback;
	currentDuration = 0.0f;
	duration = static_cast<float>(_duration.asMilliseconds());
	repeat = _repeat;
	autoDestroy = _autoDestroy;

	if (_register)
	{
		Register();
	}
}

void Timer::Register()
{
	TimerManager::GetInstance().Add(id, this);
}

void Timer::Update(const float _deltaTime)
{
	if (!isRunning) return;

	if (repeat == 0)
	{
		if (autoDestroy) Destroy();
		return;
	}

	currentDuration += _deltaTime;

	if (currentDuration >= duration)
	{
		Reset();
		if (callback) callback();
		
		if (repeat >= 1) repeat--;
	}
}

void Timer::Run(const int _repeat)
{
	Reset();
	if (_repeat != 0) repeat = _repeat;
	isRunning = true;
}

void Timer::Pause()
{
	if (this)
	{
		isRunning = false;
	}
}

void Timer::Resume()
{
	if (this)
	{
		isRunning = true;
	}
}

void Timer::Reset()
{
	currentDuration = 0.0f;
}