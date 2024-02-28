#pragma once
#include "Component.h"
#include "IManagable.h"
#include <string>
#include "Tile.h"

using namespace std;

class LightningComponent : public Component, public IManagable<string>
{
	int range;


public:
	LightningComponent(const string& _id,Entity* _owner, int _range);

	// Hérité via IManagable
	void Register() override;

public:
	virtual void Update() override;

	void UpdateLight(const vector<Tile*> _shadowTiles);
};

