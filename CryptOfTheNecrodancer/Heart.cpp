#include "Heart.h"
#include "UIImage.h"
#include "TextureManager.h"
#include "ActionMap.h"

#define PATH_HEART "FullHeart.png"
#define PATH_EMPTY_HEART "EmptyHeart.png"
#define PATH_HALFHEART "HalfHeart.png"

Heart::Heart(const string& _id, const Vector2f& _size, const Vector2f& _position) : UIImage(_id, _position, _size, "FullHeart.png")
{
	path = "FullHeart.png";
}

void Heart::UpdateLife() //Change la vie
{
	// TODO temp innpus, to remove!!
	new ActionMap("life", {
		ActionData("Vide", [this]() { TextureManager::GetInstance().Load(shape, PATH_EMPTY_HEART); }, {Event::KeyPressed, Keyboard::A}),
		ActionData("moitie", [this]() { TextureManager::GetInstance().Load(shape, PATH_HALFHEART); }, {Event::KeyPressed, Keyboard::D}),
		});
}

void Heart::UIHeart() //Change taille imageAnimation
{
	
}
