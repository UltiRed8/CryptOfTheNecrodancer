#pragma once
#include"Entity.h"

enum WallType
{
	WT_DIRT, WT_INVULNERABLE, WT_SHOP, WT_STONE
};

class Wall:public Entity
{
	WallType wallType;

public:
	Wall(const Vector2f& _position,const WallType& _type);

	string GetPathWithType(const WallType& _type) const
	{
		string _path[] = {
			"Walls.png",
			"Bedrock.png",
			"ShopWall.png",
			"StoneWall.png",
		};

		return _path[_type];
	}

public:
	void Destroy(const bool _usingBomb = false);
	void Update() override;
};

