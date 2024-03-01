#pragma once
#include "UIElement.h"
#include "UIImage.h"

enum State
{
	H_FULL, H_HALF, H_EMPTY
};

class Heart : public UIImage
{
	string path;
	State state;
	float currentLife;
	float maxLife;

public :
	void SetState(State _state) 
	{ 
		state = _state; 
	}

	string GetPathWithState() const
	{
		string _path[] = {
			"FullHeart.png",
			"HalfHeart.png",
			"EmptyHeart.png",
		};

		return _path[state];
	}

public :
	Heart(const string& _id, const Vector2f& _size, const Vector2f& _position, const State& _state, const float& _maxLife = 100);
public :
	void UpdateLife(); //Change la vie ?
	void UIHeart(); //Change taille imageAnimation
	virtual void Update(const Vector2i& _mousePosition) override;
};

