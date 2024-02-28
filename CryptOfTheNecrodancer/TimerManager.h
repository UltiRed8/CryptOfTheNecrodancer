#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Singleton.h"
#include "IManager.h"
#include "Timer.h"

using namespace sf;
using namespace std;

class TimerManager : public Singleton<TimerManager>, public IManager<string, Timer>
{
	Clock clock;
	float time;
	float lastTime;
	float lastFrameTime;
	float elapsedTime;
	float timeScale;
	float deltaTime;
	unsigned int framesCount;
	int* maxFrameRate;
	float fps;
	function<void()> renderCallback;

public:
	int* GetMaxFrameRate() const
	{
		return maxFrameRate;
	}
	void SetTimeScale(const float _timeScale)
	{
		timeScale = _timeScale;
	}
	void SetMaxFrameRate(const int _maxFrameRate)
	{
		*maxFrameRate = _maxFrameRate;
	}
	float GetFPS() const
	{
		return fps;
	}
	void ComputeFPS()
	{
		fps = 1000.0f / (time - lastFrameTime);
	}
	bool Render()
	{
		if (lastFrameTime == 0 || time - lastFrameTime >= 1000.0f / *maxFrameRate)
		{
			ComputeFPS();
			lastFrameTime = time;
			return true;
		}

		return false;
	}
	float GetDeltaTime() const
	{
		return deltaTime;
	}
	void SetRenderCallback(const function<void()>& _callback)
	{
		renderCallback = _callback;
	}

public:
	TimerManager();

private:
	void UpdateTimers();

public:
	void Update();
	void UpdateFrameRate(int _addValue);
};