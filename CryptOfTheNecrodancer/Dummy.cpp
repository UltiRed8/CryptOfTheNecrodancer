#include "Dummy.h"

#define PATH_DUMMY "Entities/Dummy.png"

Dummy::Dummy(const Vector2f& _position) : Enemy(999999999, 0, STRING_ID("Dummy"), PATH_DUMMY, _position, 0)
{

}

void Dummy::SelectDirection()
{

}