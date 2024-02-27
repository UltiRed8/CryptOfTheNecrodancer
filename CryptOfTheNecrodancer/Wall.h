#pragma once
#include"Entity.h"

enum WallType
{
	WT_GRASS,WT_INVULNERABLE,WT_SHOP
};

class Wall:public Entity
{
	WallType type;
	bool isInvulnerable;
public:
	Wall(const Vector2f& _position,bool _isInvulnerable,const WallType& _type);

	string GetPathWithType(const WallType& _type) const
	{
		string _path[] = {
			"Walls.png",
			"Bedrock.png",
			"ShopWall.png"
		};

		return _path[_type];
	}

public:

	void Update() override;
};

