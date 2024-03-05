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

public:
	vector<Drawable*> GetDrawables();
	float* GetIndex();

public :
	MenuManager();
	
public :
	void ClickAction();
	void Update();
	bool BlockPlayer();
	void InitMenu();
	void DeleteSaveDataMenu();
	void OptionsMenu();
	void CloseMenu();
	void GraphicMenu();
	void LeaveLobby();
	void LatencyMenu();
	void GameOverMenu();
	void ToggleLoading();
	void ToggleWarningSeizure();
	void ToggleEpilepsyMenu();
	void ToggleRebindMenu();
	void ToggleCredits();

private :
	void InitMenuPause();
	void InitHUD();
	void InitDeleteSaveDataMenu();
	void InitMenuOptions();
	void InitMenuClose();
	void InitGraphicMenu();
	void InitLeaveLobby();
	void InitMenuLatency();
	void InitGameOver();
	void Loading();
	void WarningSeizure();
	void InitEpilepsyMenu();
	void InitRebindMenu();
	void InitCredits();

	void Delete();
	void GoToLobby();
	void Restart();
};