#pragma once
#include"Component.h"
#include<functional>
#include<vector>
using namespace std;

class RythmComponentt : public Component
{
	function<void()> beforeCallback;
	function<void()> timedCallback;
	function<void()> afterCallback;

public:
	RythmComponentt(Entity* _owner, const function<void()>& _beforeCallback, const function<void()>& _timedCallback, const function<void()>& _afterCallback);

	// Hérité via Component
	void Update() override;
	void TimedUpdate();
	// todo a passer en callback par méthodes
};

