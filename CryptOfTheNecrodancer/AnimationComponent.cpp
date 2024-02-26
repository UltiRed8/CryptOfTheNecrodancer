#include "AnimationComponent.h"
#include <iostream>
#include "TextureManager.h"
#include "EntityManager.h"
#include"Macro.h"
AnimationComponent::AnimationComponent(Entity* _owner, const string& _path, const vector<AnimationData>& _animationsData,
	const AnimationDirection& _direction) : Component(_owner)
{
	InitAnimations(_path, _animationsData);
	currentIndex = 0;
	direction = _direction;
}


void AnimationComponent::InitAnimations(const string& _path, const vector<AnimationData>& _animationsData)
{
	for (const AnimationData& _data : _animationsData)
	{
		Sprite* _sprite = new Sprite();
		_sprite->setPosition(Vector2f(owner->GetPosition()));
		TextureManager::GetInstance().LoadSprite(_sprite, _path);
		new Animation(_data.name, this, _sprite, _data);
	}
}


void AnimationComponent::Update()
{
	if (currentIndex < 0)
	{
		cout << owner->GetID();
	}
	for (Animation* _animation:GetAllValues())
	{
		_animation->Update();
	}

	const int _newIndex = GetNextIndex(direction);
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