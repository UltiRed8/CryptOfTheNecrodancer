#pragma once

#include "Entity.h"

class Door : public Entity
{
	RectangleShape* visuals;
	int direction;

public:
	Door(const Vector2f& _position, const int _direction = -1);
	~Door();

private:
	void UpdateTexture();
	
public:
	void OpenDoor();
	void ComputeDirection();
	virtual vector<Drawable*> GetDrawables() override
	{
		vector<Drawable*> _drawables;
		_drawables.push_back(visuals);
		return _drawables;
	}
};