#include "Stair.h"
#include "Macro.h"

#define PATH_STAIR "Entities/Stairs.png"

Stair::Stair(const Vector2f& _position) : Entity(STRING_ID("Stair"), PATH_STAIR, _position)
{
	type = ET_STAIR;
	zIndex = 1;
}
