#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace sf;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define TILE_SIZE Vector2f(50, 50)

#define STRING_ID(name) name + to_string(GetUniqueID())
#define SIZE(vector) static_cast<int>(vector.size())

int GetUniqueID();

template <typename Type>
vector<Vector2f> GetAllPositions(const vector<Type>& _entities)
{
	vector<Vector2f> _allPositions;
	for (Type _entity : _entities)
	{
		_allPositions.push_back(_entity->GetPosition());
	}
	return _allPositions;
}

int Random(const int _max, const int _min = 0);


template <typename Type>
Type GetRandomElementInVector(const vector<Type> _elements)
{
	return _elements[Random((int)_elements.size() - 1, 0)];
}


Vector2i GetRandomVector2i(const int _min, const int _max);

template <typename T>
bool IsEqual(const T& _first, const T& _second, const float _minRange = 0.001f)
{
	return abs(_second - _first) <= _minRange;
}

float Length(Vector2f& _vector);

void Normalize(Vector2f& _vector);

Vector2i NormalizeDistance(const Vector2f& _vector);

float Distance(const Vector2f& _first, const Vector2f& _second);

template <typename T>
bool Contains(T _valueToFind, const vector<T>& _vector)
{
	for (T _value : _vector)
	{
		if (_value == _valueToFind) return true;
	}

	return false;
}

bool Contains(const FloatRect& _rect, const vector<Vector2f>& _vector);

bool IsNearlyEqual(Vector2f _first, Vector2f _second);

bool IsNearlyEquals(const Vector2f& _first, const Vector2f& _second, const float _accept = 0.5f);

Vector2i GetDirectionByPositions(Vector2f _entityPos, Vector2f _destination);

vector<string> Split(string _line, const string& _divider);

template <typename Type>
vector<Type> VectorToInt(const vector<string>& _vector)
{
	vector<Type> _results;
	for (const string& _element : _vector)
	{
		_results.push_back(stoi(_element));
	}
	return _results;
}

template <typename Type>
static void SetOriginCentered(Type* _element)
{
	_element->setOrigin(_element->getGlobalBounds().getSize() / 2.0f);
}

bool StringContains(const string& _id, const string& _value);

template <typename Class, typename Type>
static void EraseElements(vector<Class>& _vector, vector<Type> _element)
{
	for (Type _value : _element)
	{
		_vector.erase(remove(_vector.begin(), _vector.end(), _value), _vector.end());
	}
}

template <typename T>
static void EraseElement(vector<T>& _vector, const T& _element)
{
	_vector.erase(remove(_vector.begin(), _vector.end(), _element), _vector.end());
}

Vector2f operator * (const Vector2f& _vector, const Vector2f& _multiplier);
FloatRect operator * (const FloatRect& _rect, const Vector2f& _multiplier);
Vector2i operator * (const Vector2i& _vector, const Vector2i& _multiplier);

template <typename Type>
Type operator /= (Type& _vector, const Type& _divisor)
{
	return Type(_vector.x / _divisor.x, _vector.y / _divisor.y);
}

template <typename Type>
Type operator / (const Type& _vector, const Type& _divisor)
{
	return Type(_vector.x / _divisor.x, _vector.y / _divisor.y);
}