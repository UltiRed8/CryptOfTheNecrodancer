#pragma once

#include "Placeable.h"
#include "LightSource.h"

class Torch : public Placeable
{
	LightSource* lightSource;

public:
	Torch(const Vector2f& _position);
	~Torch();
};