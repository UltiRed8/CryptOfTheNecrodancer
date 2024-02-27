#include "TimerManager.h"

TimerManager::TimerManager()
{
	clock = Clock();
	time = 0.0f;
	lastTime = 0.0f;
	lastFrameTime = 0.0f;
	elapsedTime = 0.0f;
	timeScale = 1.0f;
	deltaTime = 0.0f;
	framesCount = 0;
	maxFrameRate = 60;
	fps = 0.0f;
}

void TimerManager::UpdateTimers()
{
	for (const auto& _pair : allValues)
	{
		_pair.second->Update(deltaTime);
	}
	GarbageCollector();
}

void TimerManager::Update()
{
	lastTime = time;

	time = static_cast<float>(clock.getElapsedTime().asMilliseconds());

	elapsedTime = time - lastTime;

	deltaTime = elapsedTime * timeScale;

	framesCount++;

	UpdateTimers();
	if (Render())
	{
		if (renderCallback)
		{
			renderCallback();
		}
	}
}
