#include "UIElement.h"
#include "Menu.h"

UIElement::UIElement(const string& _id, const Vector2f& _position) : IManagable(_id)
{
	position = _position;
	owner = nullptr;
}

void UIElement::Register()
{
	owner->Add(id, this);
}
