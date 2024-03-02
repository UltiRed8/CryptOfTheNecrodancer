#pragma once
#include "IManager.h"
#include "Singleton.h"
#include "LightSource.h"

class LightningManager : public Singleton<LightningManager>, public IManager<string, LightSource>
{
	vector<Tile*> shadowTiles;
	int brightness;

public:
	void Construct(const vector<Vector2f>& _allEntitiesPos, const int _brightness = 2);
	void ClearAll();

public:
	void Update();
};

