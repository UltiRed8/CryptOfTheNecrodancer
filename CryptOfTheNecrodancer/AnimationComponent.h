#pragma once

#include "Component.h"
#include "IManager.h"
#include "Animation.h"

class AnimationComponent : public Component, public IManager<string, Animation> 
{
	Shape* shape;
	int currentIndex;
	string playingID;

public:
	void SetPlayingID(const string& _id)
	{
		playingID = _id;
	}
	vector<Drawable*> GetDrawables() const
	{
		vector<Drawable*> _drawables;
		for (Animation* _anim : GetAllValues())
		{
			_drawables.push_back(_anim->GetShape());
		}
		return _drawables;
	}

private:
	int GetNextIndex(const string& _playingID) const
	{
		int _index = 0;
		for (Animation* _animation : GetAllValues())
		{
			if (_animation->GetData().name == _playingID) return _index;
			_index++;
		}
		return -1;
	}
public:
	Animation* GetCurrentAnimation() const
	{
		return GetAllValues()[currentIndex];
	}

public:
	AnimationComponent(Entity* _owner, const string& _path, const vector<AnimationData>& _animationsData, const string& _playingID, Shape* _shape);
	~AnimationComponent();

private:
	void InitAnimations(const string& _path, const vector<AnimationData>& _animationsData);

public:
	virtual void Update() override;
};