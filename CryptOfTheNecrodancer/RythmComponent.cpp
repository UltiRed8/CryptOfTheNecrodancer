#include "RythmComponent.h"
#include"Entity.h"
RythmComponent::RythmComponent(Entity* _owner,const vector<function<void()>>& _callbacks):Component(_owner)
{
	callbacks = _callbacks;
}

void RythmComponent::Update()
{
	for (function<void()> _callback:callbacks)
	{
		_callback();
	}
}
