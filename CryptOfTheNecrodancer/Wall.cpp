#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Map.h"
#include "WindowManager.h"

#define SOUND_DIG_DIRT "Assets/Sounds/mov_dig_dirt.ogg"
#define SOUND_DIG_FAIL "Assets/Sounds/mov_dig_fail.ogg"

Wall::Wall(const Vector2f& _position, const WallType& _type, const string& _zoneName, const bool _canSpawnWithTorch) : Placeable(STRING_ID("Wall"), "", _position)
{
	zoneName = _zoneName;

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
	torch = nullptr;
	canSpawnWithTorch = _canSpawnWithTorch;
}

void Wall::DestroyWall(const bool _usingBomb)
{
	if (IsToRemove()) return;
	bool _canBreak = true;
	if (wallType == WT_INVULNERABLE) _canBreak = false;
	if (wallType == WT_SHOP && !_usingBomb) _canBreak = false;
	if (wallType == WT_STONE && true /*TODO is not using pickaxe?*/) _canBreak = false;
	if (!_canBreak)
	{
		SoundManager::GetInstance().Play(SOUND_DIG_FAIL);
		return;
	}
	Map::GetInstance().AddFloorAt(GetPosition());
	SoundManager::GetInstance().Play(SOUND_DIG_DIRT);
	if (hasDiamond)
	{
		Map::GetInstance().AddOther(new Pickable(1, PT_DIAMOND, STRING_ID("Diamond"), GetPosition()));
	}
	if (torch)
	{
		torch->Destroy();
	}

	WindowManager::GetInstance().Shake(5);
	Destroy();
}

bool Wall::CouldBeDoor()
{
	vector<Vector2i> _directionsToCheck = {
		Vector2i(0, 1),
		Vector2i(0, -1),
		Vector2i(1, 0),
		Vector2i(-1, 0),
	};

	int _validityH = 0;
	int _validityV = 0;

	for (const Vector2i& _checking : _directionsToCheck)
	{
		Vector2f _position = GetPosition();
		_position.x += _checking.x * TILE_SIZE.x;
		_position.y += _checking.y * TILE_SIZE.y;
		Entity* _entity = Map::GetInstance().GetEntityAt(_position);
		if (!_entity) continue;
		if (_entity->GetType() == ET_FLOOR)
		{
			if (_checking.x != 0)
			{
				_validityV++;
			}
			else
			{
				_validityH++;
			}
		}
	}

	if (_validityH == 2) return true;
	if (_validityV == 2) return true;
	return false;
}

void Wall::SpawnTorch()
{
	if (!canSpawnWithTorch) return;

	if (wallType == WT_DIRT)
	{
		if (!Random(60, 0))
		{
			torch = new Torch(GetPosition());
			Map::GetInstance().AddOther(torch);
		}
	}
	else if (wallType == WT_SHOP)
	{
		if (!Random(5, 0))
		{
			torch = new Torch(GetPosition());
			Map::GetInstance().AddOther(torch);
		}
	}
}
