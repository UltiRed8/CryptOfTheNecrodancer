#include "Living.h"
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "MovementComponent.h"

Living::Living(const float _maxHp, const float _maxDammage, const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
	components.push_back(new LifeComponent(this, [&]() { cout << "Die" << endl; }, false, _maxHp));
	components.push_back(new DamageComponent(this, _maxDammage));
	components.push_back(new MovementComponent(this));
}

Living::Living(const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
}
