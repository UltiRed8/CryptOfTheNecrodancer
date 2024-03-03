#include "Placeable.h"
#include "Macro.h"

Placeable::Placeable(const string& _id, const string& _path, const Vector2f& pos) : Entity(_id, _path, pos)
{
	zIndex = 0;
}
