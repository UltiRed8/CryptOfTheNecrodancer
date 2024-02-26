#pragma once
#include "Object.h"
#include "IManagable.h"
#include "Component.h"


class Entity : public Object, public IManagable<string>
{
	vector<Component*> components;
protected:
	Shape* shape;


public:
	Shape* GetShape() const 
	{ 
		return shape;
	}

public:
	Entity(const string& _id,const string& _path, const Vector2f& _position);
	~Entity();

	// H�rit� via IManagable
	void Register() override;
};

