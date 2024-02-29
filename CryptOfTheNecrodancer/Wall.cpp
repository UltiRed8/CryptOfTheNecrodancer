#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Map.h"
#include "Diamond.h"

#define SOUND_DIG_DIRT "Assets/Sounds/mov_dig_dirt.ogg"
#define SOUND_DIG_FAIL "Assets/Sounds/mov_dig_fail.ogg"

#define DIAMOND_IN_WALL "DiamondInWall.png"


Wall::Wall(const Vector2f& _position, const WallType& _type) : Entity(STRING_ID("Wall"), "", _position)
{


	wallType = _type;

	if (wallType == WT_DIRT)
	{
		if (!Random(8, 0))
		{
			wallType = WT_STONE;
		}
	}
	
	const int _randomValue = Random(15, 0);

	TextureManager::GetInstance().LoadFromTextureSheet(shape, GetPathWithType(wallType), _randomValue, Vector2i(24, 24));
	type = ET_WALL;
}

void Wall::Update()
{
}

void Wall::DestroyWall(const bool _usingBomb)
{
	if (IsToRemove()) return;
	bool _canBreak = true;
	if (wallType == WT_INVULNERABLE) _canBreak = false;
	if (wallType == WT_SHOP && !_usingBomb) _canBreak = false;
	if (wallType == WT_STONE && true /*TODO is not using pickaxe?*/) _canBreak = false;
	if (wallType == WT_DIAMOND && true /*TODO is not using pickaxe?*/) _canBreak = false;
	if (!_canBreak)
	{
		SoundManager::GetInstance().Play(SOUND_DIG_FAIL);
		return;
	}
	Map::GetInstance().AddFloorAt(GetPosition());
	SoundManager::GetInstance().Play(SOUND_DIG_DIRT);
	if (hasDiamond)
	{
		new Diamond("Diamond", GetPosition());
	}
	Destroy();
}