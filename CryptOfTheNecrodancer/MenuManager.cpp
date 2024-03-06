#include "MenuManager.h"
#include "UIImage.h"
#include "UIText.h"
#include "ProgressBar.h"
#include "MusicManager.h"
#include "Soundmanager.h"
#include "EntityManager.h"
#include "Macro.h"
#include "CameraManager.h"
#include "TimerManager.h"
#include "Player.h"
#include "Heart.h"
#include "Map.h"
#include "LightningManager.h"
#include "WindowManager.h"
#include "RythmIndicator.h"
#include "UIAnimation.h"

#pragma region Define

#define FONT "Assets/Font/Font.ttf"
#define FONT "Assets/Font/Font.ttf"
#define WHITE_COLOR Color::White
#define CYAN_COLOR Color::Cyan

#define RYTHMHEART0 "UI/RythmHearts0.png"
#define COIN "UI/Coin.png"
#define DIAMOND "UI/Diamond.png"

#define PAUSE_MENU "UI/PauseMenu.png"
#define AREYOUSURE "UI/AreYouSure.png"
#define OPTIONS_MENU "UI/OptionsMenu.png"
#define GRAPHICS_MENU "UI/GraphicsMenu.png"
#define LATENCY_MENU "UI/LatencyMenu.png"
#define LOADING_MENU "UI/Loading.png"
#define WARNING_MENU "UI/SeizureWarningMenu.png"
#define EPILEPSY_MENU "UI/EpilepsyMenu.png"
#define REBIND_MENU "UI/RebindMenu.png"
#define CREDIT_BUTTON "UI/CreditsButton.png"

#define EMPTYCHECKBOX "UI/EmptyCheckbox.png"
#define EMPTYBAR "UI/EmptyBar.png"
#define FULLBAR "UI/FullBar.png"
#define UNICORN "UI/Unicorn.png"
#define CREDITS "UI/Credits.png"
#define REBIND "UI/Keybind.png"
#define WTF "UI/Thomas.png"

#define SOUND_START "Assets/Sounds/sfx_ui_start.ogg"
#define SOUND_EXIT "Assets/Sounds/sfx_ui_back.ogg"
#define SOUND_UP "Assets/Sounds/sfx_ui_select_up.ogg"
#define SOUND_DOWN "Assets/Sounds/sfx_ui_select_down.ogg"
#define SOUND_TOGGLE "Assets/Sounds/sfx_ui_toggle.ogg"
#define SOUND_CREDIT "Assets/Sounds/Credits.ogg"

#define LOBBY "Assets/Saved/Lobby.txt"

#define PATH_SHOVEL "UI/Shovel.png"
#define PATH_SWORD "UI/Sword.png"

#pragma endregion

vector<Drawable*> MenuManager::GetDrawables()
{
	vector<Drawable*> _drawables;
	int _currentZIndex = 0;
	bool _shouldContinue;
	do
	{
		_shouldContinue = false;
		for (Menu* _menu : GetAllValues())
		{
			int _zIndex = _menu->GetZIndex();
			if (_zIndex == _currentZIndex)
			{
				vector<Drawable*> _tempDrawables;

				if (!_menu->IsOpened()) continue;

				else
				{
					_tempDrawables = _menu->GetDrawables();
				}
				_drawables.insert(_drawables.end(), _tempDrawables.begin(), _tempDrawables.end());
			}
			else if (_zIndex == _currentZIndex + 1)
			{
				_shouldContinue = true;
			}
		}

		_currentZIndex++;
	} while (_shouldContinue);

	return _drawables;
}

float* MenuManager::GetIndex()
{
	return index;
}

MenuManager::MenuManager()
{
	window = new RenderWindow();
	index = new float(10.0f);
}

void MenuManager::ClickAction()
{
	for (Menu* _menu : GetAllValues())
	{
		if (_menu->IsOpened())
		{
			if (_menu->ClickAction())
			{
				return;
			}
		}
	}
}

void MenuManager::Update()
{
	for (Menu* _menu : GetAllValues())
	{
		if (_menu->IsOpened())
		{
			_menu->Update(window);
		}
	}
}

bool MenuManager::BlockPlayer()
{
	for (Menu* _menu : GetAllValues())
	{
		if (_menu->IsOpened())
		{
			if (_menu->GetID() != "HUD" && _menu->GetID() != "PlayerLife") return true;
		}
	}
	return false;
}

void MenuManager::InitHUD()
{
	Menu* _hud = new Menu("HUD", {
		//Rythmed Heart
		new UIImage("RythmHearts", Vector2f(SCREEN_WIDTH / 2 - 20 * 2, SCREEN_HEIGHT - 55 * 2), Vector2f(40.0f, 50.0f) * 2.0f, RYTHMHEART0),

		//Multiplier
		new UIText("CoinMultText", Vector2f(SCREEN_WIDTH / 2 - 20 * 0.5, SCREEN_HEIGHT - 55 * 0.2), WHITE_COLOR, "Coin Multiplier: " , 15, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier()),

		//Coins
		new UIImage("Coin", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 12.8), Vector2f(25.0f, 25.0f) * 2.0f, COIN),
		new UIText("CoinUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 12.5), WHITE_COLOR, "x " ,25, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetMoney()),

		//Diamond
		new UIImage("Diamond", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 11.6), Vector2f(25.0f, 25.0f) * 2.0f, DIAMOND),
		new UIText("DiamondUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 11.3), WHITE_COLOR, "x " ,25, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetDiamonds()),

		new UIImage("Shovel", Vector2f(10, 10), Vector2f(90, 99), PATH_SHOVEL),
		new UIImage("Sword", Vector2f(120, 10), Vector2f(90, 99), PATH_SWORD),


		});

	_hud->Open();
}

void MenuManager::InitMenu()
{
	window = WindowManager::GetInstance().GetWindow();
	InitLeaveLobby();
	InitMenuLatency();
	InitHUD();
	InitMenuPause();
	InitMenuOptions();
	InitMenuClose();
	InitGraphicMenu();
	InitDeleteSaveDataMenu();
	InitGameOver();
	Loading();
	WarningSeizure(); 
	InitEpilepsyMenu();
	InitCredits();
}

void MenuManager::InitMenuPause()
{
	new ActionMap("GamePaused",
		{ ActionData("Escape", [this]()
			{
				if (BlockPlayer()) return;
				MenuManager::GetInstance().Get("GamePause")->Open();
				MusicManager::GetInstance().Pause();
				for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
				{
					if (AnimationComponent* _animationComponent = _entity->GetComponent<AnimationComponent>())
					{
						for (Animation* _animation : _animationComponent->GetAllValues())
						{
							_animation->GetTimer()->Pause();
						}
					}
				}
				for (UIElement* _element : Get("HUD")->GetAllValues())
				{
					if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
					{
						_indicator->Pause();
					}
				}
			},
			{Event::KeyPressed, Keyboard::Escape}),
		ActionData("Select", [this]()
			{ MenuManager::GetInstance().ClickAction(); },
			{Event::MouseButtonPressed, Mouse::Left})});

	function<void()> _callbackContinue = [this]() {
		Get("GamePause")->Toggle();
		MusicManager::GetInstance().Unpause();
		for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
		{
			if (AnimationComponent* _animationComponent = _entity->GetComponent<AnimationComponent>())
			{
				for (Animation* _animation : _animationComponent->GetAllValues())
				{
					_animation->GetTimer()->Resume();
				}
			}
		}
		for (UIElement* _element : Get("HUD")->GetAllValues())
		{
			if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
			{
				_indicator->Resume();
			}
		}
		};
	function<void()> _callbackRestart = [this]() { Restart(); Get("GamePause")->Toggle(); };
	function<void()> _callbackOptions = [this]() { Get("GamePause")->Toggle(); OptionsMenu(); };
	function<void()> _callbackLobby = [this]() {Get("GamePause")->Toggle(); GoToLobby(); MusicManager::GetInstance().Unpause(); };
	function<void()> _callbackDelete = [this]() { DeleteSaveDataMenu(); };
	function<void()> _callbackCalibration = [this]() { Get("GamePause")->Toggle(); LatencyMenu(); };
	function<void()> _more = [this]() { MusicManager::GetInstance().IncreaseMusicPackName(); };
	function<void()> _less = [this]() { MusicManager::GetInstance().DecreaseMusicPackName(); };
	function<void()> _credit = [this]() { SoundManager::GetInstance().Play(SOUND_CREDIT); Get("GamePause")->Toggle(); ToggleCredits(); };
	function<void()> _callbackEchap = [this]() { Get("GamePause")->Toggle(); CloseMenu(); };

	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	new Menu("GamePause", {
		new UIButton("ContinueButton", Vector2f(_x, static_cast<float>(_windowY / 5)),WHITE_COLOR, CYAN_COLOR, "Continue Game", 50, FONT, SOUND_START, _callbackContinue),
		new UIButton("RestartButton", Vector2f(_x, static_cast<float>(_windowY / 3.5)), WHITE_COLOR, CYAN_COLOR, "Quick Restart", 50, FONT, SOUND_START, _callbackRestart),
		new UIButton("OptionsButton", Vector2f(_x, static_cast<float>(_windowY / 2.7)), WHITE_COLOR, CYAN_COLOR, "Options", 50, FONT, SOUND_START, _callbackOptions),
		new UIButton("ReturnLobbyButton", Vector2f(_x, static_cast<float>(_windowY / 2.2)), WHITE_COLOR, CYAN_COLOR, "Quit to Lobby", 50, FONT, SOUND_EXIT, _callbackLobby),
		new UIButton("DeleteSaveDataButton", Vector2f(_x, static_cast<float>(_windowY / 1.85)), WHITE_COLOR, CYAN_COLOR, "Delete Save Data", 50, FONT, SOUND_START, _callbackDelete),
		new UIButton("Calibration", Vector2f(_x, static_cast<float>(_windowY / 1.58)), WHITE_COLOR, CYAN_COLOR, "Calibration Latency", 50, FONT, SOUND_START, _callbackCalibration),

		new UIText("MusicPack", Vector2f(static_cast<float>(window->getSize().x / 2.3), static_cast<float>(_windowY / 1.4)), Color(172, 172,173), "Music Pack",50,FONT, true),
		new UIText("MusicPackTextUpdate", Vector2f(static_cast<float>(window->getSize().x / 1.6), static_cast<float>(_windowY / 1.4)), Color(172, 172,173), "",50,FONT, true, MusicManager::GetInstance().GetMusicPackName()),
		new UIButton("MusicPackMore", Vector2f(static_cast<float>(window->getSize().x / 1.47), static_cast<float>(_windowY / 1.41)), WHITE_COLOR, CYAN_COLOR, ">", 50, FONT, SOUND_UP, _more),
		new UIButton("MusicPackLess", Vector2f(static_cast<float>(window->getSize().x / 1.75), static_cast<float>(_windowY / 1.41)), WHITE_COLOR, CYAN_COLOR, "<", 50, FONT, SOUND_DOWN, _less),
		new UIText("MusicText", Vector2f(_x, static_cast<float>(_windowY / 1.28)), Color(161, 6,6), "For changing the Music Pack, you need to change zone",20,FONT, false),

		new UIButton("credit", Vector2f(static_cast<float>(window->getSize().x / 1.3), static_cast<float>(_windowY / 1.16)), Color(119,136,153), WHITE_COLOR, {
			new UIImage("credit", Vector2f(static_cast<float>(window->getSize().x / 1.3), static_cast<float>(_windowY / 1.16)), Vector2f(150.0f, 48.0f), CREDIT_BUTTON),
		}, SOUND_START, _credit, FloatRect(static_cast<float>(window->getSize().x / 1.3), static_cast<float>(_windowY / 1.16), 150.0f, 48.0f)),

		new UIButton("Exit", Vector2f(_x, static_cast<float>(_windowY / 1.16)), WHITE_COLOR, CYAN_COLOR, "Exit Game", 50, FONT, SOUND_EXIT, _callbackEchap),
		new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), PAUSE_MENU), }, 1);
}

void MenuManager::InitDeleteSaveDataMenu()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _delete = [&]() { Delete(); };
	function<void()> _return = [this]() { DeleteSaveDataMenu(); };

	new Menu("Delete", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("AreYouSureText1", Vector2f(_x, static_cast<float>(_windowY / 4)), WHITE_COLOR, "Are you sure you want to",35, FONT),
		new UIText("AreYouSureText2", Vector2f(_x, static_cast<float>(_windowY / 3.3)), WHITE_COLOR, "delete your save data ?",35, FONT),
		new UIButton("StayText", Vector2f(_x, static_cast<float>(_windowY / 1.8)), WHITE_COLOR, CYAN_COLOR, "No, stay here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("DeleteShit", Vector2f(_x, static_cast<float>(_windowY / 1.6)), WHITE_COLOR, CYAN_COLOR, "Yes, delete this shit", 35, FONT, SOUND_START, _delete) }, 2);
}

void MenuManager::DeleteSaveDataMenu()
{
	Get("Delete")->Toggle();
}

void MenuManager::Delete()
{
	//TODO
	//Fonction pour delete la data Save
	DeleteSaveDataMenu();
	GoToLobby();
}

void MenuManager::GoToLobby()
{
	for (UIElement* _element : Get("HUD")->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->Resume();
		}
	}
	if (Map::GetInstance().GetCurrentZone() == Z_LOBBY)	return;
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->ResetLife();
	Map::GetInstance().Open(Z_LOBBY);
}

void MenuManager::Restart()
{
	if (Map::GetInstance().GetCurrentZone() == Z_LOBBY)
	{
		MusicManager::GetInstance().Unpause();
		for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
		{
			if (AnimationComponent* _animationComponent = _entity->GetComponent<AnimationComponent>())
			{
				for (Animation* _animation : _animationComponent->GetAllValues())
				{
					_animation->GetTimer()->Resume();
				}
			}
		}
		for (UIElement* _element : Get("HUD")->GetAllValues())
		{
			if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
			{
				_indicator->Resume();
			}
		}
		return;
	}

	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->ResetLife();
	_player->GetRessources()->SetDiamonds(0);
	_player->GetRessources()->SetMoney(0);

	Map::GetInstance().QuickRestart();
}

void MenuManager::InitMenuOptions()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _activateSound = [this]() { SoundManager::GetInstance().ToggleVolume(); };
	function<void()> _activateMusic = [this]() { MusicManager::GetInstance().ToggleVolume(); };
	function<void()> _volumeUpM = [this]() { MusicManager::GetInstance().IncreaseVolume(); };
	function<void()> _volumeUpS = [this]() { SoundManager::GetInstance().IncreaseVolume(); };
	function<void()> _volumeDownM = [this]() { MusicManager::GetInstance().DecreaseVolume(); };
	function<void()> _volumeDownS = [this]() { SoundManager::GetInstance().DecreaseVolume(); };
	function<void()> _close = [this]() { OptionsMenu(); Get("GamePause")->Toggle(); };
	function<void()> _graphics = [this]() { GraphicMenu();  OptionsMenu(); };

	new Menu("Options", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), OPTIONS_MENU),
		//Menu Graphique
		new UIButton("GraphicalOptionsButton", Vector2f(_x, static_cast<float>(_windowY / 4.2)), WHITE_COLOR, CYAN_COLOR, "Graphical Options", 40, FONT, SOUND_START, _graphics),

		// Activer/Désactiver le Sound
		new UIText("ToggleSText", Vector2f(_x, static_cast<float>(_windowY / 3)), Color(172, 172,173), "Toggle Sound",40,FONT, true),
		new UIButton("CheckBoxS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3)), WHITE_COLOR, Color(0,139,139), {
			new UIImage("CheckBoxImageS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextS", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 3)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _activateSound, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3), 30.0f, 30.0f)),

		// Monter/Descendre le son du Sound
		new UIText("SoundVolText", Vector2f(_x, static_cast<float>(_windowY / 2.4)), Color(172, 172,173), "Sound Volume",40,FONT, true),
		new ProgressBar("SoundBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.12)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, SoundManager::GetInstance().GetVolume()),
		new UIButton("SoundMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.19)), WHITE_COLOR, CYAN_COLOR, ">", 50, FONT, SOUND_UP, _volumeUpS),
		new UIButton("SoundLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.19)), WHITE_COLOR, CYAN_COLOR, "<", 50, FONT, SOUND_DOWN, _volumeDownS),

		// Activer/Désactiver la musique
		new UIText("ToggleMText", Vector2f(_x, static_cast<float>(_windowY / 1.75)), Color(172, 172,173), "Toggle Music",40,FONT, true),
		new UIButton("CheckBoxM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75)), WHITE_COLOR, Color(0,139,139), {
			new UIImage("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 1.75)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _activateMusic, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75), 30.0f, 30.0f)),

		// Monter/Descendre le son de la musique
		new UIText("MusicVolText", Vector2f(_x, static_cast<float>(_windowY / 1.52)), Color(172, 172,173), "Music Volume",40,FONT, true),
		new ProgressBar("MusicBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 1.4)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, MusicManager::GetInstance().GetVolume()),
		new UIButton("MusicMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.43)), WHITE_COLOR, CYAN_COLOR, ">", 50, FONT, SOUND_UP, _volumeUpM),
		new UIButton("MusicLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 1.43)), WHITE_COLOR, CYAN_COLOR, "<", 50, FONT, SOUND_DOWN, _volumeDownM),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(_x, static_cast<float>(_windowY / 1.2)), WHITE_COLOR, CYAN_COLOR, "Done", 40, FONT, SOUND_EXIT, _close) }, 2);
}

void MenuManager::OptionsMenu()
{
	Get("Options")->Toggle();
}

void MenuManager::InitMenuClose()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _close = [&]() { window->close(); };
	function<void()> _return = [this]() { CloseMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("AreYouSure", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("AreYouSureTextClose", Vector2f(_x, static_cast<float>(_windowY / 3.3)), WHITE_COLOR, "Are you sure you want to leave?",35,FONT),
		new UIButton("StayAreYouSure", Vector2f(_x, static_cast<float>(_windowY / 1.8)), WHITE_COLOR, CYAN_COLOR, "No, stay here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("LeaveGame", Vector2f(_x, static_cast<float>(_windowY / 1.6)), WHITE_COLOR, CYAN_COLOR, "Yes, quit the game", 35, FONT, SOUND_START, _close) }, 2);
}

void MenuManager::CloseMenu()
{
	Get("AreYouSure")->Toggle();
}

void MenuManager::InitGraphicMenu()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _up = [this]() { CameraManager::GetInstance().ZoomOut(); };
	function<void()> _down = [this]() { CameraManager::GetInstance().ZoomIn(); };

	function<void()> _more = [this]() { TimerManager::GetInstance().UpdateFrameRate(10); };
	function<void()> _less = [this]() { TimerManager::GetInstance().UpdateFrameRate(-10); };

	function<void()> _resetZoom = [this]() { CameraManager::GetInstance().Reset(); };

	function<void()> _epilepsy = [this]() { ToggleEpilepsyMenu(); GraphicMenu(); };
	function<void()> _close = [this]() { OptionsMenu(); GraphicMenu(); };

	new Menu("Graphics", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), GRAPHICS_MENU),
		// Menu Special Epilepsy
		new UIButton("Epilepsy", Vector2f(_x, static_cast<float>(_windowY / 4.6)), WHITE_COLOR, CYAN_COLOR, "Epilepsy Options", 40, FONT, SOUND_EXIT, _epilepsy), 

		//View Multiplier
		new UIText("ViewText", Vector2f(_x, static_cast<float>(_windowY / 3.2)), Color(172, 172,173), "View Multiplier",40,FONT, true),
		new ProgressBar("ViewBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.72)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, CameraManager::GetInstance().GetZoomIndex(), 1.5f),
		new UIButton("ViewMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.79)), WHITE_COLOR, CYAN_COLOR, ">", 50, FONT, SOUND_UP, _up),
		new UIButton("ViewLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.79)), WHITE_COLOR, CYAN_COLOR, "<", 50, FONT, SOUND_DOWN, _down),

		// Activer/Désactiver le zoom
		new UIText("ToggleZText", Vector2f(_x, static_cast<float>(_windowY / 2)), Color(172, 172,173), "Zoom per default",40,FONT, true),
		new UIButton("CheckBoxZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2)), WHITE_COLOR, Color(0,139,139), {
			new UIImage("CheckBoxImageZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextZ", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 2)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _resetZoom, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2), 30.0f, 30.0f)),

		//SetFrameRateLimit
		new UIText("FPS", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color(172, 172,173), "Framerate Limit",40,FONT, true),
		new UIText("FPSTextUpdate", Vector2f(_x, static_cast<float>(_windowY / 1.4)), Color(172, 172,173), "",40,FONT, true, TimerManager::GetInstance().GetMaxFrameRate()),
		new UIButton("FPSMore", Vector2f(static_cast<float>(window->getSize().x / 1.7), static_cast<float>(_windowY / 1.45)), WHITE_COLOR, CYAN_COLOR, "+", 50, FONT, SOUND_UP, _more),
		new UIButton("FPSLess", Vector2f(static_cast<float>(window->getSize().x / 2.5), static_cast<float>(_windowY / 1.45)), WHITE_COLOR, CYAN_COLOR, "-", 50, FONT, SOUND_DOWN, _less),

		new UIButton("ReturnGraphics", Vector2f(_x, static_cast<float>(_windowY / 1.2)), WHITE_COLOR, CYAN_COLOR, "Done", 40, FONT, SOUND_EXIT, _close) }, 3);
}

void MenuManager::GraphicMenu()
{
	Get("Graphics")->Toggle();
}

void MenuManager::InitLeaveLobby()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _nextLevel = [&]() { LeaveLobby(); Map::GetInstance().OpenPrepared(); };
	function<void()> _return = [this]() { LeaveLobby(); };

	new Menu("LeaveLobby", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("AreYouSureText1", Vector2f(_x, static_cast<float>(_windowY / 4)), WHITE_COLOR, "You have Diamonds !",35,FONT),
		new UIText("AreYouSureText2", Vector2f(_x, static_cast<float>(_windowY / 3.3)), WHITE_COLOR, "Are you sure you want to leave the lobby?",35,FONT),
		new UIButton("StayText", Vector2f(_x, static_cast<float>(_windowY / 1.8)), WHITE_COLOR, CYAN_COLOR, "No, spend Diamonds here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("DeleteShit", Vector2f(_x, static_cast<float>(_windowY / 1.6)), WHITE_COLOR, CYAN_COLOR, "Yes ! Go to fight !", 35, FONT, SOUND_START, _nextLevel) }, 4);
}

void MenuManager::LeaveLobby()
{
	Get("LeaveLobby")->Toggle();
}

void MenuManager::InitMenuLatency()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _calibrationUp = [this]() { MusicManager::GetInstance().SetAcceptDelay(10.0f); dynamic_cast<UIText*>(Get("LatencyMenu")->Get("CalibTextUpdate"))->GetText()->setString(to_string(*MusicManager::GetInstance().GetAcceptDelay()).substr(0, to_string(*MusicManager::GetInstance().GetAcceptDelay()).find_first_of('.'))); }; //TODO
	function<void()> _calibrationDown = [this]() { MusicManager::GetInstance().SetAcceptDelay(-10.0f); dynamic_cast<UIText*>(Get("LatencyMenu")->Get("CalibTextUpdate"))->GetText()->setString(to_string(*MusicManager::GetInstance().GetAcceptDelay()).substr(0, to_string(*MusicManager::GetInstance().GetAcceptDelay()).find_first_of('.'))); }; //TODO
	function<void()> _close = [this]() { LatencyMenu(); Get("GamePause")->Toggle(); };
	
	new Menu("LatencyMenu", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), LATENCY_MENU),
		new UIText("CalibText", Vector2f(_x, static_cast<float>(_windowY / 4)), WHITE_COLOR, "Video/Audio Latency : ",35,FONT),
		new ProgressBar("CalibBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.72)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, MusicManager::GetInstance().GetAcceptDelay(), 450.0f),
		new UIButton("CalibUp", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.79)), WHITE_COLOR, CYAN_COLOR, ">", 50, FONT, SOUND_UP, _calibrationUp),
		new UIButton("CalibDown", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.79)), WHITE_COLOR, CYAN_COLOR, "<", 50, FONT, SOUND_DOWN, _calibrationDown),
		new UIText("CalibTextUpdate", Vector2f(_x, static_cast<float>(_windowY / 2)), Color(172, 172,173), "300",40,FONT, true),
		
		new UIAnimation("Unicorn", Vector2f(static_cast<float>(window->getSize().x / 2.3), static_cast<float>(_windowY / 1.7)), Vector2f(180.0f, 130.0f), UNICORN, Vector2f(51.0f, 35.0f), 5),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(_x, static_cast<float>(_windowY / 1.2)), WHITE_COLOR, CYAN_COLOR, "Done", 40, FONT, SOUND_EXIT, _close)
		}, 5);


}

void MenuManager::LatencyMenu()
{
	Get("LatencyMenu")->Toggle();
}

void MenuManager::InitGameOver()
{
	function<void()> _return = [this]() { GameOverMenu();  GoToLobby();  };
	function<void()> _restart = [&]() { GameOverMenu();  Restart(); };

	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	new Menu("Dead", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("GameOverTxt", Vector2f(_x, static_cast<float>(_windowY / 4)), WHITE_COLOR, "GAME OVER",60,FONT),
		new UIText("DeadTxt", Vector2f(_x, static_cast<float>(_windowY / 3)), WHITE_COLOR, "You are dead !",35,FONT),
		new UIButton("QuickRestart", Vector2f(_x, static_cast<float>(_windowY / 1.8)), WHITE_COLOR, CYAN_COLOR, "Quick Restart", 35, FONT, SOUND_EXIT, _restart),
		new UIButton("ReturnToLobby", Vector2f(_x, static_cast<float>(_windowY / 1.6)), WHITE_COLOR, CYAN_COLOR, "Return to Lobby", 35, FONT, SOUND_START, _return) }, 4);
}

void MenuManager::GameOverMenu()
{
	Get("Dead")->Toggle();
}

void MenuManager::Loading()
{
	unsigned int _windowY = window->getSize().y;

	new Menu("Loading", {
		new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), LOADING_MENU),
		new UIText("LoadingText", Vector2f(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 37), Color(92, 51, 18), "Opening lobby", 30, FONT),
	}, 4);
}

void MenuManager::ToggleLoading()
{
	Get("Loading")->Toggle();
}

void MenuManager::WarningSeizure()
{
	unsigned int _windowY = window->getSize().y;

	function<void()> _skip = [&]() { ToggleWarningSeizure(); };
	const Vector2f& _sizeWindow = Vector2f((float)(SCREEN_WIDTH), (float)(SCREEN_HEIGHT));

	//const string& _id, const Vector2f& _position, const Color& _unhoverColor, const Color& _hoverColor, const string& _path, const Vector2f& _imageSize, const string& _soundPath, const function<void()>& _callback)
	new Menu("WarningSeizure", { new UIButton("Warning", Vector2f(0.0f, 0.0f), Color::White, Color::White, WARNING_MENU, _sizeWindow, "", _skip), }, 4);
}

void MenuManager::InitEpilepsyMenu()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _toggleDiscoMode = [this]() { Map::GetInstance().ToggleDiscoModeEnabled(); };
	function<void()> _toggleVibration = [this]() { WindowManager::GetInstance().ToggleShakable(); };
	function<void()> _toggleRotation = [this]() { CameraManager::GetInstance().Get("PlayerCamera")->ToggleRotating(); };
	function<void()> _close = [this]() { ToggleEpilepsyMenu(); GraphicMenu(); };

	new Menu("Epilepsy", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), EPILEPSY_MENU),
		//DiscoMode
		new UIText("ToggleDMText", Vector2f(_x, static_cast<float>(_windowY / 3)), Color(172, 172, 173), "Toggle Disco Mode", 40, FONT, true),
			new UIButton("CheckBoxDM", Vector2f(static_cast<float>(window->getSize().x / 3.45), static_cast<float>(_windowY / 3)), WHITE_COLOR, Color(0, 139, 139), {
			new UIImage("CheckBoxImageDM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextDM", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 3)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _toggleDiscoMode, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3), 30.0f, 30.0f)),

		// Activer/Désactiver la Vibration
		new UIText("ToggleVText", Vector2f(_x, static_cast<float>(_windowY / 2.1)), Color(172, 172,173), "Toggle Vibration",40,FONT, true),
		new UIButton("CheckBoxV", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.1)), WHITE_COLOR, Color(0,139,139), {
			new UIImage("CheckBoxImageV", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.1)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextV", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 2.1)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _toggleVibration, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.1), 30.0f, 30.0f)),

		// Activer/Désactiver la Vibration
		new UIText("ToggleRText", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color(172, 172,173), "Toggle Rotation",40,FONT, true),
		new UIButton("CheckBoxR", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.6)), WHITE_COLOR, Color(0,139,139), {
			new UIImage("CheckBoxImageR", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.6)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxImageR", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 1.6)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _toggleRotation, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.6), 30.0f, 30.0f)),

		// Retour menu précédent
		new UIButton("Return", Vector2f(_x, static_cast<float>(_windowY / 1.2)), WHITE_COLOR, CYAN_COLOR, "Done", 40, FONT, SOUND_EXIT, _close),
	}, 3);
}

void MenuManager::InitCredits()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _callbackEchap = [this]() {  SoundManager::GetInstance().Stop(SOUND_CREDIT); ToggleCredits(); Get("GamePause")->Toggle(); };

	new Menu("Credits", { new UIAnimation("1", Vector2f(0, 0), Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)), CREDITS, Vector2f(640.0f, 360.0f), 8),
		new UIText("CTxt", Vector2f(_x, static_cast<float>(_windowY / 10)), WHITE_COLOR, "Credits",50,FONT, true),
		new UIText("C", Vector2f(static_cast<float>(window->getSize().x / 1.16), static_cast<float>(_windowY / 1.03)), Color::Red, "Copie de Crypt of The Necrodancer",20,FONT),

		new UIText("CTxtI", Vector2f(_x, static_cast<float>(_windowY / 4.5)), WHITE_COLOR, "Chef Gru : Kylari",35,FONT),
		new UIImage("Wtf", Vector2f(static_cast<float>(window->getSize().x / 6), static_cast<float>(_windowY / 4.1)), Vector2f(100.0f, 100.0f) * 2.0f, WTF),

		new UIText("CTxtA", Vector2f(_x, static_cast<float>(_windowY / 3.2)), WHITE_COLOR, "Lead Vector : UltiRed",35,FONT),

		new UIText("CTxtG", Vector2f(_x, static_cast<float>(_windowY / 2.5)), WHITE_COLOR, "Minion Jerry: Greg",35,FONT),

		new UIText("CTxtJ", Vector2f(_x, static_cast<float>(_windowY / 2)), WHITE_COLOR, "Minion Bob : Mystop",35,FONT),

		new UIText("CTxtM", Vector2f(_x, static_cast<float>(_windowY / 1.5)), WHITE_COLOR, "Intermittant Fred McDade: Medy31",35,FONT),

		// Retour menu précédent
		new UIButton("Return", Vector2f(_x, static_cast<float>(_windowY / 1.2)), WHITE_COLOR, CYAN_COLOR, "Done", 40, FONT, SOUND_EXIT, _callbackEchap)
		}, 5);

}

void MenuManager::ToggleWarningSeizure()
{
	Get("WarningSeizure")->Toggle();
}

void MenuManager::ToggleEpilepsyMenu()
{
	Get("Epilepsy")->Toggle();
}

void MenuManager::ToggleCredits()
{
	Get("Credits")->Toggle();
}
