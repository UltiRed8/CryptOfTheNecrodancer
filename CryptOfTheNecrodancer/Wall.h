#pragma once
#include"Entity.h"
#include "TextureManager.h"
#include "Macro.h"
#include "AnimationComponent.h"

#define DIAMOND_IN_WALL "DiamondInWall.png"


enum WallType
{
	WT_DIRT, WT_INVULNERABLE, WT_SHOP, WT_STONE
};

class Wall:public Entity
{
	WallType wallType;
	bool hasDiamond;

public:
	WallType GetWallType() const
	{
		return wallType;
	}
	void SetHasDiamond(const bool _hasDiamond)
	{
		hasDiamond = _hasDiamond;
		//AnimationData _animation = AnimationData("DiamondInWall", Vector2f(0, 0), Vector2f(24, 24), READ_RIGHT, ANIM_DIR_NONE, true, 3, 0.1f);
		//components.push_back(new AnimationComponent(this, GetPathWithType(wallType), { _animation }, ANIM_DIR_NONE));
		TextureManager::GetInstance().Load(shape, DIAMOND_IN_WALL);
	}
	bool GetHasDiamond()
	{
		return hasDiamond;
	}
	string GetPathWithType(const WallType& _type) const
	{
		string _path[] = {
			"Walls.png",
			"Bedrock.png",
			"ShopWall.png",
			"StoneWall.png"
		};

		return _path[_type];
	}

public:
	Wall(const Vector2f& _position,const WallType& _type);

public:
	void DestroyWall(const bool _usingBomb = false);
	void Update() override;
};

