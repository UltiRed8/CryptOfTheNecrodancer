#include "Item.h"
#include "TextureManager.h"
#include "Macro.h"
#include "TimerManager.h"
#include "Math.h"
//TODO DIRE A AXEL QUE C'EST UN ENORME CONNARD AVEC SON PTN DE BLOOM 
#define PATH_SHADOW "Entities/Shadow.png"

#define PI 3.14159265358979323846

Item::Item(const SlotType& _type, const string& _id, const Vector2f& _position) : Entity(_id, "", _position)
{
	visuals = new RectangleShape(TILE_SIZE);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);
	TextureManager::GetInstance().Load(shape, PATH_SHADOW);
	stats = ItemStats();
	type = _type;
	zIndex = 1;
	animationValue = (float)Random((const int)(floor(2.0f * (float)PI)), 0);
	animationTimer = new Timer(STRING_ID("ItemAnimation"), [this]() {
		animationValue += 0.1;
		visuals->setPosition(GetPosition() + Vector2f(0.0f, -0.5f - sin(animationValue) * 0.05f) * TILE_SIZE);
		int _opacity = (const int)(255 * ((1 - (sin(animationValue))) / 2.0f));
		_opacity = _opacity < 150 ? 150 : _opacity;
		shape->setFillColor(Color(255, 255, 255, _opacity));
		if (animationValue >= 2.0f * (float)PI)
		{
			animationValue = 0.0f;
		}
	}, seconds(0.1f), -1, false);
}

Item::~Item()
{
	animationTimer->Pause();
	animationTimer->Destroy();
}

void Item::UpdateTexture()
{
	TextureManager::GetInstance().Load(visuals, GetTexturePath());
}