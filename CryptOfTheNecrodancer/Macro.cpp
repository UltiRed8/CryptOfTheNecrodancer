#include "Macro.h"

int GetUniqueID()
{
	static int _id = 0;
	return _id++;
}

float Length(Vector2f& _vector)
{
	return sqrt(_vector.x * _vector.x + _vector.y * _vector.y);
}

void Normalize(Vector2f& _vector)
{
	_vector /= Length(_vector);
}

float Distance(const Vector2f& _first, const Vector2f& _second)
{
	return sqrtf(pow(_second.x - _first.x, 2.0f) + pow(_second.y - _first.y, 2.0f));
}
