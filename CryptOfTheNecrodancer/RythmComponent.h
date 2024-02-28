#pragma once
#include"Component.h"
#include<functional>
#include<vector>
using namespace std;
class RythmComponent : public Component
{
	function<void()> beforeCallback;
	function<void()> timedCallback;
	function<void()> afterCallback;

public:
	RythmComponent(Entity* _owner, const function<void()>& _beforeCallback, const function<void()>& _timedCallback, const function<void()>& _afterCallback);

	// Hérité via Component
	void Update() override;
	void BeforeUpdate();
	void TimedUpdate();
	void AfterUpdate();
	// todo a passer en callback par méthodes
};

