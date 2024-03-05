#pragma once
#include "UIImage.h"
#include "AnimationComponent.h"
#include <SFML/Graphics.hpp>

class UIAnimation : public UIImage
{
	AnimationComponent* animation;
public :
	UIAnimation(const string& _id, const Vector2f& _position,
		const Vector2f& _size, const string& _path, const Vector2f& _sizeAnim, const int _lenght);
	~UIAnimation();

};

