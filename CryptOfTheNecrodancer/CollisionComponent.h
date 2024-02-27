#pragma once
#include<functional>
#include<vector>
#include"Component.h"
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;
struct CollisionReaction
{
	function<void(Entity* _entity)> callback;

	CollisionReaction(const function<void(Entity* _entity)>& _callback)
	{
		callback = _callback;
	}

};
class CollisionComponent:public Component
{
public:
	CollisionComponent(Entity* _owner);

public:
	// Hérité via Component
	void Update() override;
	void CheckCollision(const vector<CollisionReaction>& _reaction);

};

