#include "Macro.h"

int GetUniqueID()
{
	static int _id = 0;
	return _id++;
}

int Random(const int _max, const int _min)
{
	random_device _rd;
	mt19937 _seed(_rd());
	uniform_int_distribution<int> _distribution(_min, _max);
	return _distribution(_seed);
}

Vector2i GetRandomVector2i(const int _min, const int _max)
{
	const int _sizeX = Random(_max, _min);
	const int _sizeY = Random(_max, _min);
	return Vector2i(_sizeX, _sizeY);
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

bool Contains(const FloatRect& _rect, const vector<Vector2f>& _vector)
{
	for (const Vector2f& _position : _vector)
	{
		if (_rect.contains(_position))
		{
			return true;
		}
	}

	return false;
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

Vector2f operator * (const Vector2f& _vector, const Vector2f& _multiplier)
{
	return Vector2f(_vector.x * _multiplier.x, _vector.y * _multiplier.y);
}

Vector2i operator * (const Vector2i& _vector, const Vector2i& _multiplier)
{
	return Vector2i(_vector.x * _multiplier.x, _vector.y * _multiplier.y);
}