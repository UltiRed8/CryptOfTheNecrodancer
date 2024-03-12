#include "Tile.h"
#include "Macro.h"

#define C_PURPLE Color(255, 62, 216, 255)
#define C_GREEN Color(53, 233, 136, 255)

#define PATH_WATER "Dungeons/Water.png"

Tile::Tile(const string& _tilePath, const Vector2f& _position, const EntityType& _type) : Placeable(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
	type = _type;
	if (_type == ET_SHADOW)
	{
		shape->setFillColor(Color::Black);
		shape->move(Vector2f(0.0f, -0.5f) * TILE_SIZE);
		zIndex = 3;
	}
	if (_type == ET_WATER)
	{
		TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER, Random(2, 0), Vector2i(26, 26));
	}
}

void Tile::InvertAlpha(const bool _reset)
{
	shape->setOutlineThickness(0.f);
	if (_reset)
	{
		baseColor = defaultColor;
		shape->setFillColor(baseColor);
		return;
	}
	baseColor.a = baseColor.a == 255 ? 200 : 255;
	shape->setFillColor(baseColor);
}

void Tile::ToggleHighlight(const int _alphaValueToHighlight)
{
	if (type == ET_WATER)
	{
		return;
	}
	if (defaultColor.a == _alphaValueToHighlight)
	{
		shape->setFillColor(defaultColor.a == 255 ? C_PURPLE : C_GREEN);
		Color _color = Color::Black;
		_color.a = 100;
		shape->setOutlineThickness(-5.f);
		shape->setOutlineColor(_color);
	}
	else
	{
		shape->setFillColor(defaultColor);
		shape->setOutlineThickness(0.f);
	}
}