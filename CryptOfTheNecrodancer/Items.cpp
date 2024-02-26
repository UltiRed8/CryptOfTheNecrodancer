#include "Items.h"
#include "TextureManager.h"

Items::Items(const string& _id, const string& _path, const Vector2f& _position) : Entity(_id, _path, _position)
{

}

Items::Items(const string& _id, const Vector2f& _position) : Entity(_id, "", _position)
{
}

void Items::SetPath(Shape* _shape, const string& _path)
{
	TextureManager::GetInstance().Load(_shape, _path);
}
