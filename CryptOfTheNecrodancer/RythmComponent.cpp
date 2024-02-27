#include "RythmComponent.h"
#include"Entity.h"
#include"TimerManager.h"
RythmComponent::RythmComponent(Entity* _owner, const function<void()>& _beforeCallback, const function<void()>& _timedCallback, const function<void()>& _afterCallback) : Component(_owner)
{
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