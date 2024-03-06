#include "Item.h"
#include "TextureManager.h"

Item::Item(const SlotType& _type, const string& _id, const Vector2f& _position) : Entity(_id, "", _position)
{
	type = _type;
}


Item::Item(const SlotType& _type, const string& _id, const string& _path, const Vector2f& _position) : Entity(_id, _path, _position)
{
	type = _type;
}

void Item::SetPath(Shape* _shape, const string& _path)
{
	TextureManager::GetInstance().Load(_shape, _path);
}
