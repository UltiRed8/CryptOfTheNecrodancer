#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(Entity* _owner, const string& _path, const vector<AnimationData>& _animationsData, const string& _playingID, Shape* _shape) : Component(_owner)
{
	shape = _shape;
	InitAnimations(_path, _animationsData);
	currentIndex = -1;
	playingID = _playingID;
	Update();
}

AnimationComponent::~AnimationComponent()
{
	for (Animation* _animation : GetAllValues())
	{
		_animation->Stop();
		delete _animation;
	}
}

void AnimationComponent::InitAnimations(const string& _path, const vector<AnimationData>& _animationsData)
{
	for (const AnimationData& _data : _animationsData)
	{
		new Animation(_data.name, this, shape, _data);
	}
}

void AnimationComponent::Update()
{
	const int _newIndex = GetNextIndex(playingID);
	if (currentIndex == _newIndex) return;

	if (currentIndex != -1)
	{
		Animation* _currentAnimation = GetCurrentAnimation();
		if (!_currentAnimation) return;
		_currentAnimation->Stop();
		currentIndex = -1;
	}

	Animation* _animation = GetAllValues()[_newIndex];
	_animation->Start();
	currentIndex = _newIndex;
}