#pragma once
#include"Component.h"
#include<functional>
#include<vector>
using namespace std;
class RythmComponent:public Component
{
	vector<function<void()>> callbacks;
public:
	RythmComponent(Entity* _owner, const vector<function<void()>>& _callbacks);

	// Hérité via Component
	void Update() override;
};

