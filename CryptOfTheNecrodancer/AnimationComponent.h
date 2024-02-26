#pragma once
#include "Component.h"
#include "IManager.h"
#include "Animation.h"

class AnimationComponent : public Component, public IManager<string, Animation> 
{
	int currentIndex;
	AnimationDirection direction;

public:
	void SetDirection(const AnimationDirection& _direction)
	{
		direction = _direction;
	}

private:
	int GetNextIndex(const AnimationDirection& _direction) const
	{
		int _index = 0;
		for (Animation* _animation : GetAllValues())
		{
			if (_animation->GetData().direction == _direction) return _index;
			_index++;
		}
		return -1;
	}
public:
	Animation* GetCurrentAnimation() const
	{
		return GetAllValues()[currentIndex];
	}

public:
	AnimationComponent(Entity* _owner, const string& _path, const vector<AnimationData>& _animationsData,
		const AnimationDirection& _direction);

private:
	void InitAnimations(const string& _path, const vector<AnimationData>& _animationsData);

public:
	virtual void Update() override;
};