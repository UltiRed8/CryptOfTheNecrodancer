#pragma once

#include"Enemy.h"

#define PATH_NORMAL_BAT "Entities/BatSprite.png"
#define PATH_RED_BAT "Entities/bat_red.png"
#define PATH_BLACK_BAT "Entities/bat_black.png"

enum BatType
{
    B_NONE, B_NORMAL, B_RED, B_BLACK
};

class Bat:public Enemy
{
protected:
    BatType batType;

public:
    Bat(const float _maxHp, const float _maxDammage, const Vector2f& _position, const int _droppedCoins);

public:
	void SelectDirection();
};

struct NormalBat : public Bat
{
    NormalBat(const Vector2f& _position) : Bat(100.f, 50.0f, _position, 2)
    {
        SetTexture(PATH_NORMAL_BAT, visuals);
        batType = B_NORMAL;
        directionsPatern = {
        Vector2i(1,0),
        Vector2i(-1,0),
        Vector2i(0,1),
        Vector2i(0,-1),
        };
    }
};

struct RedBat : public Bat
{
    RedBat(const Vector2f& _position) : Bat(100.f, 100.0f, _position, 3)
    {
        SetTexture(PATH_RED_BAT, visuals);
        batType = B_RED;
        directionsPatern = {
        Vector2i(1,0),
        Vector2i(-1,0),
        Vector2i(0,1),
        Vector2i(0,-1),
        };
    }
};

struct BlackBat : public Bat
{
    BlackBat(const Vector2f& _position) : Bat(100.f, 100.0f, _position, 4)
    {
        SetTexture(PATH_BLACK_BAT, visuals);
        batType = B_BLACK;
        directionsPatern = {
        Vector2i(1,0),
        Vector2i(-1,0),
        Vector2i(0,1),
        Vector2i(0,-1),
        };
    }
};