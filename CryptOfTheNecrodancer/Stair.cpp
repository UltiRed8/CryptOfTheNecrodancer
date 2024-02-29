#include "Stair.h"
#include "Macro.h"

Stair::Stair(const string& _path, const Vector2f& _position) : Entity(STRING_ID("Stair"), _path, _position)
{
	type = ET_STAIR;
}
