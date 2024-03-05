#include "UIAnimation.h"

UIAnimation::UIAnimation(const string& _id, const Vector2f& _position, const Vector2f& _size, const string& _path) : UIImage(_id, _position, _size, _path)
{
	animation = new AnimationComponent(nullptr, vector<AnimationData> {
		AnimationData("Animation", Vector2f(51, 35), 0, 5, 0.1f, true),
	}, "Animation", shape);
}

UIAnimation::~UIAnimation()
{
	delete animation;
}
