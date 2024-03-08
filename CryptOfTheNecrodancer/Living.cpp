#include "Living.h"
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "MovementComponent.h"
#include "Pickable.h"
#include "Map.h"

Living::Living(const float _maxHp, const float _maxDammage, const string& _path, const string& _id, const Vector2f& _pos, const bool _shouldResetDirection) : Entity(_id, _path, _pos)
{
	components.push_back(new MovementComponent(this, 1, _shouldResetDirection));
	components.push_back(new LifeComponent(this, [&]() { 
		this->DieEvent();
	}, false, _maxHp));
	components.push_back(new DamageComponent(this, _maxDammage));
}

Living::Living(const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
	
}
