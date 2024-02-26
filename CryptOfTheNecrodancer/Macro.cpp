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
Vector2i NormalizeDistance(const Vector2f& _vector)
{
	return Vector2i(static_cast<int>(_vector.x / Distance(Vector2f(0.0f, 0.0f), _vector)), static_cast<int>(_vector.y / Distance(Vector2f(0.0f, 0.0f), _vector)));
}

float Distance(const Vector2f& _first, const Vector2f& _second)
{
	return sqrtf(pow(_second.x - _first.x, 2.0f) + pow(_second.y - _first.y, 2.0f));
}

bool IsNearlyEqual(Vector2f _first, Vector2f _second)
{
	return Vector2i(_first) == Vector2i(_second);
}

Vector2i GetDirectionByPositions(Vector2f _entityPos, Vector2f _destination)
{
	if (IsNearlyEqual(_entityPos, _destination)) return Vector2i(0, 0);

	return NormalizeDistance(_destination - _entityPos);
}
