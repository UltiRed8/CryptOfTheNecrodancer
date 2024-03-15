#pragma once

#include"Enemy.h"

class Dummy :public Enemy
{
public:
    Dummy(const Vector2f& _position);

private:
    virtual void SelectDirection() override;
};