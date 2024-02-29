#pragma once
#include "Entity.h"
#include "AnimationComponent.h"
#include "RythmComponent.h"

class Hephaestus : public Entity
{
	int currentSoundIndex;
public:
	Hephaestus(const Vector2f& _position);

public:
	virtual void Update() override;
};

