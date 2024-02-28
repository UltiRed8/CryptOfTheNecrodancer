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


int Random(const int _max, const int _min = 0);

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
bool IsNearlyEqual(Vector2f _first, Vector2f _second);

Vector2i GetDirectionByPositions(Vector2f _entityPos, Vector2f _destination);

template <typename Type>
static void SetOriginCentered(Type* _element)
{
	_element->setOrigin(_element->getGlobalBounds().getSize() / 2.0f);
}

template <typename Class, typename Type>
static void EraseElements(vector<Class>& _vector, vector<Type> _element)
{
	for (Type _value : _element)
	{
		_vector.erase(remove(_vector.begin(), _vector.end(), _value), _vector.end());
	}
}
