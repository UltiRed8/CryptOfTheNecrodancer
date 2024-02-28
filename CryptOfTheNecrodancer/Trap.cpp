#include "Trap.h"
#include "Macro.h"

Trap::Trap(const string& _path, const Vector2f& _position) : Entity(STRING_ID("Trap"), _path, _position)
{
	type = ET_TRAP;
}
