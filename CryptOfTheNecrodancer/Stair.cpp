#include "Stair.h"
#include "SoundManager.h"
#include "WindowManager.h"
#include "Map.h"

#define PATH_STAIR "Dungeons/Stairs.png"
#define PATH_STAIR_LOCKED "Dungeons/StairsLocked.png"
#define PATH_STAIR_LOCKED_BOSS "Dungeons/StairsLockedMiniBoss.png"

#define SOUND_LOCKED_STAIRS "Assets/Sounds/sfx_error_ST.ogg"

Stair::Stair(const Vector2f& _position, const Zone& _zoneToLoad, const LockType& _lockType, const string& _textValue) : Entity(STRING_ID("Stair"), "", _position)
{
	type = ET_STAIR;
	zIndex = 1;
	lock = _lockType;
	text = nullptr;
	zoneToLoad = _zoneToLoad;
	if (!_textValue.empty())
	{
		SetText(_textValue);
	}
	ChangeTexture();
}

Stair::~Stair()
{
	if (text)
	{
		delete text;
	}
}

void Stair::ChangeTexture()
{
	const string _paths[] = {
		PATH_STAIR,
		PATH_STAIR_LOCKED,
		PATH_STAIR_LOCKED_BOSS,
	};

	TextureManager::GetInstance().Load(shape, _paths[lock]);
}

bool Stair::OpenZone()
{
	if (IsLocked())
	{
		SoundManager::GetInstance().Play(SOUND_LOCKED_STAIRS);
		WindowManager::GetInstance().Shake();
		return false;
	}
	Map::GetInstance().Prepare(zoneToLoad);
	if (Map::GetInstance().GetCurrentZone() == Z_LOBBY)
	{
		if (*dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetDiamonds() > 0)
		{
			Menu* _leaveMenu = MenuManager::GetInstance().Get("LeaveLobby");
			_leaveMenu->Open();
			return false;
		}
	}
	Map::GetInstance().OpenPrepared();
	return true;
}