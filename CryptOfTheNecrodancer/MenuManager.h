#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Menu.h"
#include "Calibration.h"

#include <string>

class MenuManager : public Singleton<MenuManager>, public IManager<string, Menu>
{
	RenderWindow* window;
	Vector2f factor;
	float* index;
	Calibration* calibration;

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
	void OptionsMenu();
	void CloseMenu();
	void GraphicMenu();
	void InitDeleteSaveDataMenu();
	void DeleteSaveDataMenu();
	void LeaveLobby();
	void LatencyMenu();
	void GameOverMenu();
	void ToggleLoading();
	void ToggleWarningSeizure();
	void ToggleEpilepsyMenu();
	void ToggleCredits();
	void ToggleKeyboard();
	void ToggleCalibration();

private :
	void InitMenuPause();
	void InitHUD();
	void InitMenuOptions();
	void InitMenuClose();
	void InitGraphicMenu();
	void InitLeaveLobby();
	void InitMenuLatency();
	void InitGameOver();
	void Loading();
	void WarningSeizure();
	void InitEpilepsyMenu();
	void InitCredits();
	void InitKeyboard();
	void InitCalibration();
	void Delete();
	void GoToLobby();
	void Restart();
};