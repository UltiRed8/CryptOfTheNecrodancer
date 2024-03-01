#pragma once
#include "Component.h"
#include "IManagable.h"
#include <string>
#include "Tile.h"

using namespace std;

class LightSource : public IManagable<string>
{
	int range;
	Entity* owner;

public:
	LightSource(const string& _id,Entity* _owner, int _range);

	// H�rit� via IManagable
	void Register() override;

public:


	void UpdateLight(const vector<Tile*> _shadowTiles);
};

