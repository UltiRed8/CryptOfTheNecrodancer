#include "RythmComponent.h"
#include"Entity.h"
#include"TimerManager.h"

RythmComponentt::RythmComponentt(Entity* _owner, const function<void()>& _beforeCallback, const function<void()>& _timedCallback, const function<void()>& _afterCallback) : Component(_owner)
{
	beforeCallback = _beforeCallback;
	timedCallback = _timedCallback;
	afterCallback = _afterCallback;
}

void RythmComponentt::Update()
{

}

void RythmComponentt::TimedUpdate()
{
	if (timedCallback)
	{
		timedCallback();
	}
}