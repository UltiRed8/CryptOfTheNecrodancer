#include "RythmComponent.h"
#include"Entity.h"
#include"TimerManager.h"
RythmComponent::RythmComponent(Entity* _owner, const vector<function<void()>>& _callbacks) : Component(_owner)
	beforeCallback = _beforeCallback;
	timedCallback = _timedCallback;
	afterCallback = _afterCallback;
}

void RythmComponent::Update()
{

}

void RythmComponent::BeforeUpdate()
{
	if (beforeCallback)
	{
		beforeCallback();
	}
}

void RythmComponent::TimedUpdate()
{
	if (timedCallback)
	{
		timedCallback();
	}
}

void RythmComponent::AfterUpdate()
{
	if (afterCallback)
	{
		afterCallback();
	}
}