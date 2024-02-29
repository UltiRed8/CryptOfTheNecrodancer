#pragma once
#include "Tile.h"

#define PATH_DIAMOND "Diamond.png"
#define PATH_DIAMOND_PICKED_UP "Assets/Sounds/sfx_pickup_diamond.ogg"

class Diamond : public Tile
{

public:
	Diamond(const string& _id, const Vector2f& _position);
public:
	void PickUpDiamond();
};

