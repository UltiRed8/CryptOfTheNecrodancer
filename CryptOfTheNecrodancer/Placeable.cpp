#include "Placeable.h"
#include "Macro.h"
#include "Timer.h"

Placeable::Placeable(const string& _id, const string& _path, const Vector2f& pos) : Entity(_id, _path, pos)
{
	zIndex = 0;
}

void Placeable::DestroyAfterDelay(const float _seconds)
{
	new Timer("DestroyWaterAfterStun", [this]() {
		Destroy();
	}, seconds(_seconds), 1, true);
}
