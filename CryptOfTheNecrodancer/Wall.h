#pragma once
#include "Placeable.h"
#include "TextureManager.h"
#include "Macro.h"
#include "AnimationComponent.h"

#define DIAMOND_IN_WALL "Dungeons/Zone1/DiamondInWall.png"


enum WallType
{
	WT_DIRT, WT_INVULNERABLE, WT_SHOP, WT_STONE
};

class Wall:public Placeable
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

		TextureManager::GetInstance().Load(shape, DIAMOND_IN_WALL);

		components.push_back(new AnimationComponent(this, {
			AnimationData("DiamondInWall", Vector2f(24, 24), Random(2, 0) * 3, 2, 0.1f, false),
		}, "DiamondInWall", shape));
	}
	bool GetHasDiamond()
	{
		return hasDiamond;
	}
	string GetPathWithType(const WallType& _type) const
	{
		string _path[] = {
			"Dungeons/Zone1/Walls.png",
			"Dungeons/Bedrock.png",
			"Dungeons/ShopWall.png",
			"Dungeons/Zone1/StoneWall.png"
		};

		return _path[_type];
	}

public:
	Wall(const Vector2f& _position,const WallType& _type);

public:
	void DestroyWall(const bool _usingBomb = false);
};

