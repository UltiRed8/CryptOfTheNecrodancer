#pragma once
class Entity;
class Component
{
protected:
	Entity* owner;
public:
	Entity* GetOwner()
	{
		return owner;
	}
public:
	Component(Entity* _owner);

public:
	virtual void Update() = 0 ;
};

