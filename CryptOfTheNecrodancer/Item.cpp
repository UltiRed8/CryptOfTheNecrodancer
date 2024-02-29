#include "Item.h"
#include "TextureManager.h"

Item::Item(const string& _id, const string& _path, const Vector2f& _position) : Entity(_id, _path, _position)
{

}

Item::Item(const string& _id, const Vector2f& _position) : Entity(_id, "", _position)
{
}

void Item::SetPath(Shape* _shape, const string& _path)
{
	TextureManager::GetInstance().Load(_shape, _path);
}
