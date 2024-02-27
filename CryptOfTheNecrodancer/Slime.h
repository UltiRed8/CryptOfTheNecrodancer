#pragma once
#include "Enemy.h"
#include "SlimeType.h"
#include"MovementComponent.h"


#define PATH_GREEN_SLIME "GreenSlimeSprite.png"
#define PATH_BLUE_SLIME "BlueSlimeSprite.png"
#define PATH_ORANGE_SLIME "OrangeSlimeSprite.png"



class Slime : public Enemy
{
	SlimeType slimeType;
public:
	Slime(const Vector2f& _position, const string _path);

public:
	void DieEvent() override;
	void SelectDirection();
	void SelectDirection(const int _direction);
	void SetNextDirection();
	virtual void UpdateRythm();
};


struct GreenSlime : public Slime
{

	GreenSlime(const Vector2f& _position) : Slime(_position, PATH_GREEN_SLIME)
	{
	}

};

struct BlueSlime : public Slime
{
	BlueSlime(const Vector2f& _position) : Slime(_position, PATH_BLUE_SLIME)
	{
	}
};

struct OrangeSlime : public Slime
{
	OrangeSlime(const Vector2f& _position) : Slime(_position, PATH_ORANGE_SLIME)
	{
	}
};
