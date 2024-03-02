#include "LightningManager.h"

void LightningManager::Construct(const vector<Vector2f>& _allEntitiesPos, const int _brightness)
{
	const int _size = static_cast<int>(_allEntitiesPos.size());
	for (int _index = 0; _index < _size; _index++)
	{
		shadowTiles.push_back(new Tile("",_allEntitiesPos[_index], ET_SHADOW));
	}
	brightness = _brightness;
}

void LightningManager::ClearAll()
{
	for (Tile* _tile : shadowTiles)
	{
		_tile->Destroy();
	}
	shadowTiles.clear();
}

void LightningManager::Update()
{
	for (Tile* _tile : shadowTiles)
	{
		Shape* _shape = _tile->GetShape();

		Color _color = _shape->getFillColor();
		_color.a = 255 * ((100.0f - (float)brightness) / 100.0f);
		_shape->setFillColor(_color);
	}

	for (LightSource* _lightning : GetAllValues())
	{
		if (_lightning->IsToRemove())
		{
			continue;
		}
		_lightning->UpdateLight(shadowTiles);
	}
	GarbageCollector();
}
