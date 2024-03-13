#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Map.h"
#include "WindowManager.h"

#define SOUND_DIG_DIRT "Assets/Sounds/mov_dig_dirt.ogg"
#define SOUND_DIG_FAIL "Assets/Sounds/mov_dig_fail.ogg"
#define SOUND_DIG_STONE "Assets/Sounds/mov_dig_stone.ogg"
#define SOUND_DIG_DIAMOND "Assets/Sounds/sfx_dig_diamond.ogg"
#define COIN "UI/Coin.png"

Wall::Wall(const Vector2f& _position, const WallType& _type, const string& _zoneName, const bool _canSpawnWithTorch) : Placeable(STRING_ID("Wall"), "", _position)
{
	visuals = new RectangleShape(TILE_SIZE);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);

	zIndex = 2;

	shape->setFillColor(Color::Transparent);

	zoneName = _zoneName;

	wallType = _type;

	if (wallType == WT_DIRT)
	{
		if (!Random(8, 0))
		{
			wallType = WT_STONE;
		}
	}
	
	textureIndex = Random(15, 0);

	if (wallType == WT_DIRT)
	{
		TextureManager::GetInstance().LoadFromTextureSheet(visuals, "Dungeons/" + zoneName + "/Walls.png", textureIndex, Vector2i(24, 24));
	}
	else
	{
		TextureManager::GetInstance().Load(visuals, GetPathWithType(wallType));
	}

	type = ET_WALL;
	torch = nullptr;
	canSpawnWithTorch = _canSpawnWithTorch;
}

Wall::~Wall()
{
	delete visuals;
}

void Wall::DestroyWall(const bool _usingBomb, const bool _canShake)
{
	if (IsToRemove()) return;
	bool _canBreak = true;
	if (wallType == WT_INVULNERABLE) _canBreak = false;
	if (wallType == WT_SHOP && !_usingBomb) _canBreak = false;
	if (wallType == WT_STONE && !_usingBomb && ((Player*)(EntityManager::GetInstance().Get("Player")))->GetDigLevel() <= 1) _canBreak = false;
	if (!_canBreak)
	{
		SoundManager::GetInstance().Play(SOUND_DIG_FAIL);
		return;
	}
	Map::GetInstance().AddFloorAt(GetPosition());
	SoundManager::GetInstance().Play(wallType == WT_DIRT ? SOUND_DIG_DIRT : SOUND_DIG_STONE);
	if (wallType  == WT_SHOP)
	{
		new Pickable(25, PT_COIN, STRING_ID("Coin"), GetPosition(), COIN);
	}
	if (hasDiamond)
	{
		SoundManager::GetInstance().Play(SOUND_DIG_DIAMOND);
		Map::GetInstance().AddOther(new Pickable(1, PT_DIAMOND, STRING_ID("Diamond"), GetPosition()));
	}
	if (torch)
	{
		torch->Destroy();
	}

	if (_canShake)
	{
		WindowManager::GetInstance().Shake(25);
	}
	Entity* _entity = Map::GetInstance().GetEntityAt(GetPosition() + (Vector2f(0.0f, -1.0f) * TILE_SIZE));
	if (_entity)
	{
		if (Wall* _wall = dynamic_cast<Wall*>(_entity))
		{
			_wall->Enable3D();
		}
	}

	Map::GetInstance().RemoveWall(this);
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
		if (!Random(30, 0))
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

void Wall::Enable3D()
{
	shape->setFillColor(Color::White);

	if (wallType == WT_SHOP)
	{
		TextureManager::GetInstance().Load(shape, "Dungeons/ShopWall3D.png");
	}
	else if (wallType == WT_STONE)
	{
		TextureManager::GetInstance().Load(shape, "Dungeons/" + zoneName + "/StoneWall3D.png");
	}
	else if (wallType == WT_INVULNERABLE)
	{
		TextureManager::GetInstance().Load(shape, "Dungeons/Bedrock3D.png");
	}
	else
	{
		TextureManager::GetInstance().LoadFromTextureSheet(shape, "Dungeons/" + zoneName + "/3d.png", textureIndex, Vector2i(24, 24));
	}
}