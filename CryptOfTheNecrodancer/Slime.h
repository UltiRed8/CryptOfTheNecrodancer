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
    Slime(const float _maxHp,const float _maxDammage,const Vector2f& _position, const string& _path, const int _droppedCoins);

public:
    void SetNextDirection();
    virtual void SelectDirection() override;
};


struct GreenSlime : public Slime
{

    GreenSlime(const Vector2f& _position) : Slime(100.f,5000.f,_position, PATH_GREEN_SLIME, 1)
    {
		slimeType = S_GREEN; 
        directionsPatern = { Vector2i(0, 0) };
    }

};

struct BlueSlime : public Slime
{
    BlueSlime(const Vector2f& _position) : Slime(200.f,100.f,_position, PATH_BLUE_SLIME, 2)
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
    OrangeSlime(const Vector2f& _position) : Slime(100.f,50.f,_position, PATH_ORANGE_SLIME,2)
    {
		slimeType = S_ORANGE;
        directionsPatern = {
            Vector2i(0,-1),
            Vector2i(1,0),
            Vector2i(0,1),
            Vector2i(-1,0),
        };
		const int _size = static_cast<const int>(directionsPatern.size());
		indexPatern = Random(_size - 1);
    }
};

