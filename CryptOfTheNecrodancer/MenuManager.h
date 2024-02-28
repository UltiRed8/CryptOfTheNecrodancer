#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Menu.h"

#include <string>

class MenuManager : public Singleton<MenuManager>, public IManager<string, Menu>
{
	RenderWindow* window;
	Vector2f factor;
	float* index;
	int fps;

public:
	vector<Drawable*> GetDrawables();
	float* GetIndex();

public :
	MenuManager();
	
public :
	void ClickAction();
	void Update();

	#pragma region MenuPause

	void InitMenu(RenderWindow* _window);

	void InitMenuPause();

	void InitDeleteSaveDataMenu();
	void DeleteSaveDataMenu();
	void Delete();

	void GoToLobby();

	void InitMenuOptions();
	void OptionsMenu();

	void InitMenuClose();
	void CloseMenu();

	void InitGraphicMenu();
	void GraphicMenu();

	void ZoomIn();
	void ZoomOut();

	#pragma endregion
};