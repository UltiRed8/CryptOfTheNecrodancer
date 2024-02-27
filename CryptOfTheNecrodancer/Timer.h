#pragma once

#include "IManagable.h"

#include <functional>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Timer : public IManagable<string>
{
	function<void()> callback;
	float currentDuration;
	float duration;
	int repeat;
	bool isRunning;
	bool autoDestroy;

public:
	void SetDuration(const Time& _duration)
	{
		duration = static_cast<float>(_duration.asMilliseconds());
	}
	bool IsRunning() const
	{
		return isRunning;
	}

public:
	template <class Class, typename... Args>
	Timer(const string& _id, Class* _owner, void (Class::* _callback)(Args...), const Time& _duration,
		const int _repeat = -1, const bool _autoDestroy = false, const Args&... _args) : IManagable(_id)
	{
		callback = [_owner, _callback, _args...]()
		{
			(_owner->*_callback)(_args...);
		};

		currentDuration = 0.0f;
		duration = static_cast<float>(_duration.asMilliseconds());
		repeat = _repeat;
		autoDestroy = _autoDestroy;

		Register();
	}

	Timer(const string& _id, const function<void()>& _callback, const Time& _duration,
		  const int _repeat = -1, const bool _autoDestroy = false);

private:
	virtual void Register() override;

public:
	void Update(const float _deltaTime);
	void Run(const int _repeat = 0);
	void Pause();
	void Resume();
	void Reset();
};