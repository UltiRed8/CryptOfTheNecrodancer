#pragma once
#include<functional>
using namespace std;
struct CollisionReaction
{
	function<void()> callback;

	CollisionReaction(function<void()> _callback)
	{
		callback = _callback;
	}

};
class CollisionComponent
{
};

