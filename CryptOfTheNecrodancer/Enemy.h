#pragma once
#include"Entity.h"
class Enemy:public Entity
{
protected:
	int indexPatern;
	vector<Vector2i>directionsPatern;

public:
	Enemy(const string& _id, const string& _path, const Vector2f& _position);

public:
	virtual void DieEvent()=0;
};

