#include "LightSource.h"
#include "LightningManager.h"

LightSource::LightSource(const string& _id, Entity* _owner, int _range) : IManagable(_id)
{
	Register();
	range = _range;
	owner = _owner;
	//SetToRemove();
}

void LightSource::Register()
{
	LightningManager::GetInstance().Add(id, this);
}

void LightSource::UpdateLight(const vector<Tile*> _shadowTiles)
{
	if (IsToRemove())
	{
		return;
	}
	const float _smoothness = 50;
	for (int _index = 0; _index < _smoothness; _index++)
	{
		const float _gap =	(float)range / _smoothness;
		const float _range = range - _gap * _index;
		const float _x = owner->GetPosition().x - _range;
		const float _y = owner->GetPosition().y - _range;
		const Vector2f _halfSize = owner->GetShape()->getGlobalBounds().getSize() / 2.f;
		const Vector2f _startPos = Vector2f(_x + _halfSize.x, _y + _halfSize.y);


		const float _sizeX = _range * 2;
		const float _sizeY = _range * 2;
		const Vector2f _size = Vector2f(_sizeX, _sizeY);
		FloatRect _rect = FloatRect(_startPos, _size);

		for (Tile* _shadowTile : _shadowTiles)
		{
			Shape* _shape = _shadowTile->GetShape();
			if (_rect.intersects(_shape->getGlobalBounds()))
			{

				Color _color = _shape->getFillColor();
				int _alpha = (int)_color.a - (int)(255 / _smoothness);
				if (_alpha <= 0)
				{
					_alpha = 0;
				}
				_color.a = _alpha;
				
				_shape->setFillColor(_color);
			}

		}
	}
}
