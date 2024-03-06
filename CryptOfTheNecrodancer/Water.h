#pragma once
#include "Placeable.h"

#define PATH_WATER "Dungeons/Water.png"


class Water : public Placeable
{
public:
	Water(const Vector2f& _position);
};

