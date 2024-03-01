#pragma once
#include "IManager.h"
#include "Singleton.h"
#include "LightSource.h"

class LightningManager : public Singleton<LightningManager>, public IManager<string, LightSource>
{
	vector<Tile*> shadowTiles;

public:
	void Construct(const vector<Vector2f>& _allEntitiesPos);

	void ClearAll();
public:
	void Update();
};

