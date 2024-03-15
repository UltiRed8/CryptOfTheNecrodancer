#include "UIAnimation.h"

UIAnimation::UIAnimation(const string& _id, const Vector2f& _position, const Vector2f& _size, const string& _path, const Vector2f& _sizeAnim, const int _lenght) : UIImage(_id, _position, _size, _path)
{
	animation = new AnimationComponent(nullptr, vector<AnimationData> {
		AnimationData("Animation", _sizeAnim, 0, _lenght, 0.1f, true),
	}, "Animation", shape);
}

UIAnimation::~UIAnimation()
{
	delete animation;
}
