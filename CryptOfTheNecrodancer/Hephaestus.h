#pragma once
#include "NPC.h"
#include "AnimationComponent.h"
#include "RythmComponent.h"

class Hephaestus : public NPC
{
	int currentSoundIndex;
public:
	Hephaestus(const Vector2f& _position);

public:
	virtual void Update() override;
};
