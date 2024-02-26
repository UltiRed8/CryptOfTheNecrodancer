#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"

using namespace sf;
using namespace std;

enum ProgressType
{
	PT_CENTER,
	PT_LEFT,
	PT_TOP,
	PT_RIGHT,
	PT_BOTTOM
};

class ProgressBar
{
	ProgressType type;
	Shape* foreground;
	Shape* background;
	float maxValue;
	float currentValue;

public:
	void SetValue(const float _newValue)
	{
		currentValue = _newValue > maxValue ? maxValue : _newValue < 0 ? 0 : _newValue;
		Update();
	}
	void ChangeValue(const float _byAmount)
	{
		const float _newValue = currentValue + _byAmount;
		currentValue = _newValue > maxValue ? maxValue : _newValue < 0 ? 0 : _newValue;
		Update();
	}
	void SetProgressType(const ProgressType& _type)
	{
		type = _type;
		UpdateOriginAndPosition();
	}

public:
	ProgressBar(const ProgressType& _type, const Vector2f& _position, const Vector2f& _size, const string& _emptyPath, const string& _fullPath, const float _maxValue = 100.0f);
	~ProgressBar();

private:
	void Update();
	void UpdateOriginAndPosition();
	IntRect MakeRect(const float _percent);

public:
	vector<Drawable*> GetDrawables() const;
	void InitTextures(const string& _full, const string& _empty);
};