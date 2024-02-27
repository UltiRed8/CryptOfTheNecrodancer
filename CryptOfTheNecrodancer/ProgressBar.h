#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "UIElement.h"

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

class ProgressBar : public UIElement
{
	ProgressType type;
	Shape* foreground;
	Shape* background;
	float maxValue;
	float* currentValue;

public:
	void SetValue(const float _newValue)
	{
		*currentValue = _newValue > maxValue ? maxValue : _newValue < 0 ? 0 : _newValue;
		Update(Vector2i(0,0));
	}
	void ChangeValue(const float _byAmount)
	{
		const float _newValue = *currentValue + _byAmount;
		*currentValue = _newValue > maxValue ? maxValue : _newValue < 0 ? 0 : _newValue;
		Update(Vector2i(0, 0));
	}
	void SetProgressType(const ProgressType& _type)
	{
		type = _type;
		UpdateOriginAndPosition();
	}

public:
	ProgressBar(const ProgressType& _type, const Vector2f& _position, const Vector2f& _size, 
		const string& _emptyPath, const string& _fullPath, float* _currentValue, const float _maxValue = 100.0f);
	~ProgressBar();

private:
	void UpdateOriginAndPosition();
	virtual void Update(const Vector2i& _mousePosition) override;
	IntRect MakeRect(const float _percent);

public:
	virtual vector<Drawable*> GetDrawables() override;
	void InitTextures(const string& _full, const string& _empty);
};