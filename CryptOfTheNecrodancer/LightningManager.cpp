#include "LightningManager.h"

void LightningManager::Construct(const vector<Vector2f>& _allEntitiesPos)
{
	const int _size = static_cast<int>(_allEntitiesPos.size());
	for (int _index = 0; _index < _size; _index++)
	{
		shadowTiles.push_back(new Tile("",_allEntitiesPos[_index],ET_SHADOW));
	}
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
		_color.a = 250;
		_shape->setFillColor(_color);
	}

	for (LightningComponent* _lightning : GetAllValues())
	{
		_lightning->UpdateLight(shadowTiles);
	}
	GarbageCollector();
}
