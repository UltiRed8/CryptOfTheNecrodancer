#pragma once
#include "Enemy.h"
#include"MovementComponent.h"

#define PATH_GREEN_SLIME "SlimeSprite.png"
#define PATH_BLUE_SLIME "BlueSlimeSprite.png"
#define PATH_ORANGE_SLIME "OrangeSlimeSprite.png"

enum SlimeType
{
    S_NONE,S_BLUE, S_GREEN, S_ORANGE
};

class Slime : public Enemy
{
protected:
    SlimeType slimeType;
public:
    Slime(const Vector2f& _position, const string _path);

public:
    void DieEvent() override;
    void SetNextDirection();
    virtual void UpdateRythm();
};


struct GreenSlime : public Slime
{

    GreenSlime(const Vector2f& _position) : Slime(_position, PATH_GREEN_SLIME)
    {
		slimeType = S_GREEN; 
        directionsPatern = { Vector2i(0, 0) };
    }

};

struct BlueSlime : public Slime
{
    BlueSlime(const Vector2f& _position) : Slime(_position, PATH_BLUE_SLIME)
    {
		slimeType = S_BLUE;
        directionsPatern = {
            Vector2i(0,-1),
            Vector2i(0,1),
        };

        cooldown = 2;
    }
};

struct OrangeSlime : public Slime
{
    OrangeSlime(const Vector2f& _position) : Slime(_position, PATH_ORANGE_SLIME)
    {
		slimeType = S_ORANGE;
        directionsPatern = {
            Vector2i(0,-1),
            Vector2i(1,0),
            Vector2i(0,1),
            Vector2i(-1,0),
        };

		indexPatern = Random(directionsPatern.size() - 1);
    }
};

