#pragma once
#include "UIImage.h"
#include "Macro.h"

enum RythmIndicatorDirection
{
	RID_RIGHT, RID_LEFT
};

class RythmIndicator : public UIImage
{
	RythmIndicatorDirection directionType;
	Vector2f direction;
	Vector2f speed;
	float opacity;

public:
	Vector2f GetPositionWithDirection(const RythmIndicatorDirection& _direction) const
	{
		const float _y = SCREEN_HEIGHT - 55 * 1.2765f;
		vector<Vector2f> _position = {
			Vector2f(0,_y),
			Vector2f(SCREEN_WIDTH,_y),
		};

		return _position[_direction];
	}

	void SetDirectionWithDirectionType() 
	{
		direction = directionType == RID_RIGHT ? Vector2f(1, 0) : Vector2f(-1, 0);
	}

public:
	RythmIndicator(const RythmIndicatorDirection& _direction,Menu* _owner,const string& _path);

public:
	virtual void Update(const Vector2i& _mousePosition) override;
};

