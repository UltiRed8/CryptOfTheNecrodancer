#include "Door.h"
#include "Macro.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Map.h"

#define PATH_DOOR_V "door_side.png"
#define PATH_DOOR_H "door_front.png"
#define SOUND_DOOR_OPEN "Assets/Sounds/obj_door_open.ogg"

Door::Door(const Vector2f& _position, const int _direction) : Entity(STRING_ID("Door"), "", _position)
{
	type = ET_DOOR;
	zIndex = 1;
	direction = _direction;
	if (direction != -1)
	{
		UpdateTexture();
	}
}

void Door::ComputeDirection()
{
	const Vector2f& _doorPosition = GetPosition();

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
		Vector2f _position = _doorPosition;
		_position.x += _checking.x * TILE_SIZE.x;
		_position.y += _checking.y * TILE_SIZE.y;
		Entity* _entity = Map::GetInstance().GetEntityAt(_position);
		if (!_entity) continue;
		if (_entity->GetType() == ET_WALL)
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

	direction = (_validityH > _validityV);

	UpdateTexture();
}

void Door::UpdateTexture()
{
	TextureManager::GetInstance().Load(shape, direction == 0 ? PATH_DOOR_H : PATH_DOOR_V);
}

void Door::OpenDoor()
{
	SoundManager::GetInstance().Play(SOUND_DOOR_OPEN);
	Destroy();
}