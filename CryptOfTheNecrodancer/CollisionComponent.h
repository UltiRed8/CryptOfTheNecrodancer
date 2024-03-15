#pragma once
#include<functional>
#include<vector>
#include"Component.h"
#include<SFML/Graphics.hpp>
#include"Entity.h"

using namespace std;
using namespace sf;
struct CollisionReaction
{
	function<bool(Entity* _entity)> callback;
	EntityType type;

	CollisionReaction(const EntityType& _type, const function<bool(Entity* _entity)>& _callback)
	{
		callback = _callback;
		type = _type;
	}

};
class CollisionComponent:public Component
{
public:
	CollisionComponent(Entity* _owner);

public:
	void Update() override;
	void CheckCollision(const vector<CollisionReaction>& _reaction);
};