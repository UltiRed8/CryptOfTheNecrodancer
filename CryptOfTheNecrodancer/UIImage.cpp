#include "UIImage.h"
#include "TextureManager.h"

UIImage::UIImage(const Vector2f& _position, const Vector2f& _size, const string& _path) : UIElement(_position)
{
	shape = new RectangleShape(_size);
	shape->setPosition(_position);
	texturePath = _path;
	Init();
}

UIImage::~UIImage()
{
	delete shape;
}

void UIImage::Init()
{
	InitTexture();
}

void UIImage::InitTexture()
{
	TextureManager::GetInstance().Load(shape, texturePath);
}

vector<Drawable*> UIImage::GetDrawables()
{
	vector<Drawable*> _drawables;
	_drawables.push_back(shape);
	return _drawables;
}

void UIImage::Update(const Vector2i& _mousePosition)
{
	// TODO enlever?
}