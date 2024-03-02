#pragma once
#include "Living.h"

enum NPCType
{
	NPC_SHOPKEEPER, NPC_MERLIN, NPC_HEPHAESTUS
};

class NPC : public Living
{
	NPCType type;
	int currentCooldown;

public:
	NPC(const NPCType& _type, const Vector2f& _position);

private:
	void ChangeTexture();
	void UpdateAnimation();

public:
	virtual void DieEvent() override;
	virtual void Update() override;
};

