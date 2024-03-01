#pragma once
#include "IManager.h"
#include "Singleton.h"
#include "LightSource.h"

class LightningManager : public IManager<string, LightSource>, public Singleton<LightningManager>
{
	vector<Tile*> shadowTiles;

public:
	void Construct(const vector<Vector2f>& _allEntitiesPos);

	void ClearAll();
public:
	void Update();
};

