#pragma once
#include "UIElement.h"
#include "UIImage.h"

class Heart : public UIElement
{
	string path;
	UIImage* heart;

public :
	Heart(const string& _id, const Vector2f& _position);
public :
	void UpdateLife(); //Change la vie ?
	void UIHeart(); //Change taille imageAnimation
	virtual void Update(const Vector2i& _mousePosition) override;
	virtual vector<Drawable*> GetDrawables() override;
};

