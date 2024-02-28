#pragma once
#include "Object.h"
#include "IManagable.h"
#include "Component.h"


enum EntityType
{
	ET_NONE, ET_FLOOR, ET_WALL, ET_ENEMY, ET_PLAYER, ET_ITEM, ET_STAIR
};

class Entity : public Object, public IManagable<string>
{
protected:
	vector<Component*> components;
	Shape* shape;
	EntityType type;

public:
	EntityType GetType() const
	{
		return type;
	}
	Shape* GetShape() const 
	{ 
		return shape;
	}
	template <typename Type>
	Type* GetComponent()
	{
		for (Component* _component : components)
		{
			if (Type* _type = dynamic_cast<Type*>(_component))
			{
				return _type;
			}
		}
		return nullptr;
	}
	Vector2f GetPosition()const 
	{
		return shape->getPosition();
	}

public:
	Entity(const string& _id,const string& _path, const Vector2f& _position);
	~Entity();

	// Hérité via IManagable
	void Register() override;
	virtual void Update();
};

