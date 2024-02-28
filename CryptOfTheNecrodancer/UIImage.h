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
	RectangleShape* GetShape() const
	{
		return shape;
	}

public:
	UIImage(const string& _id, const Vector2f& _position, const Vector2f& _size, const string& _path);
	~UIImage();

private:
	void Init();
	void InitTexture();
	virtual void Update(const Vector2i& _mousePosition) override;

public:
	virtual vector<Drawable*> GetDrawables() override;
};