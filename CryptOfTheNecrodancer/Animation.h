#pragma once

#include "IManagable.h"
#include "Timer.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class AnimationComponent;

struct AnimationData
{
	bool shouldLoop;
	int stop;
	float timeBetween;
	string name;
	int start;
	Vector2f size;
	Vector2i startPosition;

	AnimationData() = default;
	AnimationData(const string& _name, const Vector2f& _spriteSize, const int _startIndex,
		const int _amountOfParts, const float _timeBetween, const bool _shouldLoop)
	{
		startPosition = Vector2i();
		name = _name;
		size = _spriteSize;
		start = _startIndex;
		shouldLoop = _shouldLoop;
		stop = _amountOfParts;
		timeBetween = _timeBetween;
	}
};

class Animation : public IManagable<string>
{
	AnimationComponent* owner;
	int currentIndex;
	string path;
	AnimationData data;
	Shape* shape;
	Timer* timer;

public:
	void SetDuration(const float _duration)
	{
		data.timeBetween = _duration / (data.stop + 1);
		if (timer)
		{
			timer->SetDuration(seconds(data.timeBetween));
		}
	}
	Timer* GetTimer() const
	{
		return timer;
	}
	AnimationData GetData() const
	{
		return data;
	}
	bool CanNext() const
	{
		return currentIndex < data.stop;
	}
	Shape* GetShape() const
	{
		return shape;
	}

public:
	Animation(const string& _name, AnimationComponent* _owner, Shape* _shape, const AnimationData& _data);

private:
	virtual void Register() override;
	void SetNext();
	Vector2i GetNewStart();

public:
	void Replay();
	void Start();
	void Reset();
	void Stop();
};