#include "MenuManager.h"

vector<Drawable*> MenuManager::GetDrawables()
{
	vector<Drawable*> _drawables;
	for (Menu* _menu : GetAllValues())
	{
		if (!_menu->IsOpened()) continue;
		const vector<Drawable*>& _tempDrawables = _menu->GetDrawables();
		_drawables.insert(_drawables.end(), _tempDrawables.begin(), _tempDrawables.end());
	}
	return _drawables;
}
