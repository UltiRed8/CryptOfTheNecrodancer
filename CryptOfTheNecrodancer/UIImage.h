#pragma once

#include "UIElement.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class UIImage : public UIElement
{
	RectangleShape* shape;
	string texturePath;

public:
	UIImage(const Vector2f& _position, const Vector2f& _size, const string& _path);
	~UIImage();

private:
	void Init();
	void InitTexture();

public:
	virtual vector<Drawable*> GetDrawables() override;
};