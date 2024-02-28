#include "Heart.h"
#include "UIImage.h"
#include "TextureManager.h"

Heart::Heart(const string& _id, const Vector2f& _position) : UIElement(_id, _position)
{
	path = "FullHeart.png";
	heart = new UIImage("Heart", _position, Vector2f(25.0f, 25.0f) * 2.0f, path);
}

void Heart::UpdateLife() //Change la vie
{
	if (Event::KeyPressed && Keyboard::A)
	{
		path = "HalfHeart.png";
	}

	if (Event::KeyPressed && Keyboard::D)
	{
		path = "EmptyHeart.png";
	}
}

void Heart::UIHeart() //Change taille imageAnimation
{
	
}

void Heart::Update(const Vector2i& _mousePosition)
{
}

vector<Drawable*> Heart::GetDrawables()
{
	vector<Drawable*> _drawables = heart->GetDrawables();
	return _drawables;
}

