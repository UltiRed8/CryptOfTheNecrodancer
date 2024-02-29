#pragma once
#include"Entity.h"
#include "TextureManager.h"
#include "Macro.h"
#include "AnimationComponent.h"

enum WallType
{
	WT_DIRT, WT_INVULNERABLE, WT_SHOP, WT_STONE, WT_DIAMOND
};

class Wall:public Entity
{
	WallType wallType;
	bool hasDiamond;

public:
	Wall(const Vector2f& _position,const WallType& _type);

	string GetPathWithType(const WallType& _type) const
	{
		string _path[] = {
			"Walls.png",
			"Bedrock.png",
			"ShopWall.png",
			"StoneWall.png",
			"DiamondInWall.png",
		};

		return _path[_type];
	}
	void SetHasDiamond(const bool _hasDiamond)
	{
		hasDiamond = _hasDiamond;
		wallType = WT_DIAMOND;
		AnimationData _animation = AnimationData("DiamondInWall", Vector2f(0, 0), Vector2f(24, 24), READ_RIGHT, ANIM_DIR_NONE, true, 3, 0.1f);
		components.push_back(new AnimationComponent(this, GetPathWithType(wallType), { _animation }, ANIM_DIR_NONE));
		// TextureManager::GetInstance().LoadFromTextureSheet(shape, GetPathWithType(wallType), Random(3, 0), Vector2i(24, 24));
	}
	bool GetHasDiamond()
	{
		return hasDiamond;
	}


public:
	void DestroyWall(const bool _usingBomb = false);
	void Update() override;
};

