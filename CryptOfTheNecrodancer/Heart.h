#pragma once
#include "UIElement.h"
#include "UIImage.h"

class Heart : public UIImage
{
	string path;

public :
	Heart(const string& _id, const Vector2f& _size, const Vector2f& _position);
public :
	void UpdateLife(); //Change la vie ?
	void UIHeart(); //Change taille imageAnimation
};

