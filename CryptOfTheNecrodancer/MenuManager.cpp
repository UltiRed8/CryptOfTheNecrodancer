#include "MenuManager.h"

void MenuManager::Open(const string& _id, RenderWindow* _window)
{
	if (Exist(_id))
	{
		Get(_id)->Open(_window);
	}
}
