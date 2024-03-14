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

bool IsNearlyEquals(const Vector2f& _first, const Vector2f& _second, const float _accept)
{
	return abs(_first.x - _second.x) < _accept && abs(_first.y - _second.y) < _accept;
}

Vector2i GetDirectionByPositions(Vector2f _entityPos, Vector2f _destination)
{
	if (IsNearlyEqual(_entityPos, _destination)) return Vector2i(0, 0);

	return NormalizeDistance(_destination - _entityPos);
}

vector<string> Split(string _line, const string& _divider)
{
	vector<string> _results;

	size_t _pos = 0;

	while ((_pos = _line.find(_divider)) != string::npos)
	{
		_pos = _line.find(_divider);
		_results.push_back(_line.substr(0, _pos));
		_line = _line.substr(_pos + _divider.length());
	}
	_results.push_back(_line);

	return _results;
}

bool Contain(const string& _id, const string& _value)
{
	return false;
}

bool StringContains(const string& _id, const string& _value)
{

	if (_id.find(_value) != string::npos)
	{
		return true;
	}
	return false;

}

Vector2f operator * (const Vector2f& _vector, const Vector2f& _multiplier)
{
	return Vector2f(_vector.x * _multiplier.x, _vector.y * _multiplier.y);
}

Vector2i operator * (const Vector2i& _vector, const Vector2i& _multiplier)
{
	return Vector2i(_vector.x * _multiplier.x, _vector.y * _multiplier.y);
}

FloatRect operator * (const FloatRect& _rect, const Vector2f& _multiplier)
{
	return FloatRect(_rect.getPosition().x * _multiplier.x, _rect.getPosition().y * _multiplier.y, _rect.getSize().x * _multiplier.x, _rect.getSize().y * _multiplier.y);
}