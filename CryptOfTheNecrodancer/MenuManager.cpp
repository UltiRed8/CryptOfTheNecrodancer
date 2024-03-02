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

#define FONT "Assets/Font/Font.ttf"
#define RYTHMHEART0 "UI/RythmHearts0.png"
#define COIN "UI/Coin.png"
#define DIAMOND "UI/Diamond.png"

#define PAUSE_MENU "UI/PauseMenu.png"
#define AREYOUSURE "UI/AreYouSure.png"
#define OPTIONS_MENU "UI/OptionsMenu.png"
#define GRAPHICS_MENU "UI/GraphicsMenu.png"
#define LATENCY_MENU "UI/LatencyMenu.png"

#define EMPTYCHECKBOX "UI/EmptyCheckbox.png"
#define EMPTYBAR "UI/EmptyBar.png"
#define FULLBAR "UI/FullBar.png"

#define SOUND_START "Assets/Sounds/sfx_ui_start.ogg"
#define SOUND_EXIT "Assets/Sounds/sfx_ui_back.ogg"
#define SOUND_UP "Assets/Sounds/sfx_ui_select_up.ogg"
#define SOUND_DOWN "Assets/Sounds/sfx_ui_select_down.ogg"
#define SOUND_TOGGLE "Assets/Sounds/sfx_ui_toggle.ogg"

#define LOBBY "Assets/Saved/Lobby.txt"

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

void MenuManager::InitHUD()
{
	Menu* _hud = new Menu("HUD", {
		//Rythmed Heart
		new UIImage("RythmHearts", Vector2f(SCREEN_WIDTH / 2 - 20 * 2, SCREEN_HEIGHT - 55 * 2), Vector2f(40.0f, 50.0f) * 2.0f, RYTHMHEART0),

		//Multiplier
		new UIText("CoinMultText", Vector2f(SCREEN_WIDTH / 2 - 20 * 0.5, SCREEN_HEIGHT - 55 * 0.2), Color::White, "Coin Multiplier: " , 15, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier()),

		//Coins
		new UIImage("Coin", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 12.8), Vector2f(25.0f, 25.0f) * 2.0f, COIN),
		new UIText("CoinUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 12.5), Color::White, "x " ,25, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetMoney()),

		//Diamond
		new UIImage("Diamond", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 11.6), Vector2f(25.0f, 25.0f) * 2.0f, DIAMOND),
		new UIText("DiamondUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 11.3), Color::White, "x " ,25, FONT, false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetDiamonds()),
		});

	_hud->Open();
}

void MenuManager::InitInventory()
{
	
}

void MenuManager::InitMenu(RenderWindow* _window)
{
	window = _window;
	InitLeaveLobby();
	InitMenuLatency();
	InitHUD();
	InitMenuPause();
	InitMenuOptions();
	InitMenuClose();
	InitGraphicMenu();
	InitDeleteSaveDataMenu();
}

void MenuManager::InitMenuPause()
{
	new ActionMap("GamePaused",
		{ ActionData("Escape", [this]()
			{
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
			},
			{Event::KeyPressed, Keyboard::Escape}),
		ActionData("Select", [this]()
			{ MenuManager::GetInstance().ClickAction(); },
			{Event::MouseButtonPressed, Mouse::Left}) });

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
	}; //TODO Fix
	function<void()> _callbackRestart = [this]() {}; //TODO
	function<void()> _callbackOptions = [this]() { Get("GamePause")->Toggle(); OptionsMenu(); };
	function<void()> _callbackLobby = [this]() {Get("GamePause")->Toggle(); GoToLobby(); MusicManager::GetInstance().PrepareMain("Lobby", 130, false, true); };
	function<void()> _callbackDelete = [this]() { DeleteSaveDataMenu(); };
	function<void()> _callbackCalibration = [this]() { Get("GamePause")->Toggle(); LatencyMenu(); };
	function<void()> _callbackEchap = [this]() { Get("GamePause")->Toggle(); CloseMenu(); };
	
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	new Menu("GamePause", {
		new UIButton("ContinueButton", Vector2f(_x, static_cast<float>(_windowY / 5)),Color::White, Color::Cyan, "Continue Game", 50, FONT, SOUND_START, _callbackContinue),
		new UIButton("RestartButton", Vector2f(_x, static_cast<float>(_windowY / 3.5)), Color::White, Color::Cyan, "Quick Restart", 50, FONT, SOUND_START, _callbackRestart),
		new UIButton("OptionsButton", Vector2f(_x, static_cast<float>(_windowY / 2.7)), Color::White, Color::Cyan, "Options", 50, FONT, SOUND_START, _callbackOptions),
		new UIButton("ReturnLobbyButton", Vector2f(_x, static_cast<float>(_windowY / 2.2)), Color::White, Color::Cyan, "Quit to Lobby", 50, FONT, SOUND_EXIT, _callbackLobby),
		new UIButton("DeleteSaveDataButton", Vector2f(_x, static_cast<float>(_windowY / 1.85)), Color::White, Color::Cyan, "Delete Save Data", 50, FONT, SOUND_START, _callbackDelete),
		new UIButton("Calibration", Vector2f(_x, static_cast<float>(_windowY / 1.58)), Color::White, Color::Cyan, "Calibration Latency", 50, FONT, SOUND_START, _callbackCalibration),
		new UIButton("Exit", Vector2f(_x, static_cast<float>(_windowY / 1.3)), Color::White, Color::Cyan, "Exit Game", 50, FONT, SOUND_EXIT, _callbackEchap),
		new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), PAUSE_MENU),}, 1);
}

void MenuManager::InitDeleteSaveDataMenu()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _delete = [&]() { Delete(); };
	function<void()> _return = [this]() { DeleteSaveDataMenu(); };

	new Menu("Delete", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("AreYouSureText1", Vector2f(_x, static_cast<float>(_windowY / 4)), Color::White, "Are you sure you want to",35, FONT),
		new UIText("AreYouSureText2", Vector2f(_x, static_cast<float>(_windowY / 3.3)), Color::White, "delete your save data ?",35, FONT),
		new UIButton("StayText", Vector2f(_x, static_cast<float>(_windowY / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("DeleteShit", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color::White, Color::Cyan, "Yes, delete this shit", 35, FONT, SOUND_START, _delete) }, 2);
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
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetRessources()->SetMoney(0);
	Map::GetInstance().DeleteAll();
	LightningManager::GetInstance().ClearAll();
	Map::GetInstance().SetCurrentZone(CL_Lobby);
	Map::GetInstance().Load(LOBBY);
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
	function<void()> _graphics = [this]() { GraphicMenu();  Get("Options")->Toggle(); };

	new Menu("Options", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), OPTIONS_MENU),
		//Menu Graphique
		new UIButton("GraphicalOptionsButton", Vector2f(_x, static_cast<float>(_windowY / 4.2)), Color::White, Color::Cyan, "Graphical Options", 40, FONT, SOUND_START, _graphics),

		// Activer/Désactiver le Sound
		new UIText("ToggleSText", Vector2f(_x, static_cast<float>(_windowY / 3)), Color(172, 172,173), "Toggle Sound",40,FONT, true),
		new UIButton("CheckBoxS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextS", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 3)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _activateSound, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 3), 30.0f, 30.0f)),

		// Monter/Descendre le son du Sound
		new UIText("SoundVolText", Vector2f(_x, static_cast<float>(_windowY / 2.4)), Color(172, 172,173), "Sound Volume",40,FONT, true),
		new ProgressBar("SoundBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.12)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, SoundManager::GetInstance().GetVolume()),
		new UIButton("SoundMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.19)), Color::White, Color::Cyan, ">", 50, FONT, SOUND_UP, _volumeUpS),
		new UIButton("SoundLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.19)), Color::White, Color::Cyan, "<", 50, FONT, SOUND_DOWN, _volumeDownS),

		// Activer/Désactiver la musique
		new UIText("ToggleMText", Vector2f(_x, static_cast<float>(_windowY / 1.75)), Color(172, 172,173), "Toggle Music",40,FONT, true),
		new UIButton("CheckBoxM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 1.75)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _activateMusic, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.75), 30.0f, 30.0f)),

		// Monter/Descendre le son de la musique
		new UIText("MusicVolText", Vector2f(_x, static_cast<float>(_windowY / 1.52)), Color(172, 172,173), "Music Volume",40,FONT, true),
		new ProgressBar("MusicBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 1.4)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, MusicManager::GetInstance().GetVolume()),
		new UIButton("MusicMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 1.43)), Color::White, Color::Cyan, ">", 50, FONT, SOUND_UP, _volumeUpM),
		new UIButton("MusicLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 1.43)), Color::White, Color::Cyan, "<", 50, FONT, SOUND_DOWN, _volumeDownM),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(_x, static_cast<float>(_windowY / 1.2)), Color::White, Color::Cyan, "Done", 40, FONT, SOUND_EXIT, _close) }, 2);
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
		new UIText("AreYouSureTextClose", Vector2f(_x, static_cast<float>(_windowY / 3.3)), Color::White, "Are you sure you want to leave?",35,FONT),
		new UIButton("StayAreYouSure", Vector2f(_x, static_cast<float>(_windowY / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("LeaveGame", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color::White, Color::Cyan, "Yes, quit the game", 35, FONT, SOUND_START, _close) }, 2);
}

void MenuManager::CloseMenu()
{
	Get("AreYouSure")->Toggle();
}

void MenuManager::InitGraphicMenu()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	Vector2f _index;
	function<void()> _up = [this]() { CameraManager::GetInstance().ZoomOut(); };
	function<void()> _down = [this]() { CameraManager::GetInstance().ZoomIn(); };

	function<void()> _more = [this]() { TimerManager::GetInstance().UpdateFrameRate(10); };
	function<void()> _less = [this]() { TimerManager::GetInstance().UpdateFrameRate(-10); };

	function<void()> _resetZoom = [this]() { CameraManager::GetInstance().Reset(); };

	function<void()> _close = [this]() { OptionsMenu(); Get("Graphics")->Toggle(); };

	new Menu("Graphics", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), GRAPHICS_MENU),
		//View Multiplier
		new UIText("ViewText", Vector2f(_x, static_cast<float>(_windowY / 3.2)), Color(172, 172,173), "View Multiplier",40,FONT, true),
		new ProgressBar("ViewBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.72)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, CameraManager::GetInstance().GetZoomIndex(), 1.5f),
		new UIButton("ViewMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.79)), Color::White, Color::Cyan, ">", 50, FONT, SOUND_UP, _up),
		new UIButton("ViewLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.79)), Color::White, Color::Cyan, "<", 50, FONT, SOUND_DOWN, _down),
		
		// Activer/Désactiver le Sound
		new UIText("ToggleZText", Vector2f(_x, static_cast<float>(_windowY / 2)), Color(172, 172,173), "Zoom per default",40,FONT, true),
		new UIButton("CheckBoxZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2)), Vector2f(30.0f, 30.0f), EMPTYCHECKBOX),
			new UIText("CheckBoxTextZ", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(_windowY / 2)), Color(0,139,139), "X", 40, FONT, false)
		}, SOUND_TOGGLE, _resetZoom, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2), 30.0f, 30.0f)),

		//SetFrameRateLimit
		new UIText("FPS", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color(172, 172,173), "Framerate Limit",40,FONT, true),
		new UIText("FPSTextUpdate", Vector2f(_x, static_cast<float>(_windowY / 1.4)), Color(172, 172,173), "",40,FONT, true, TimerManager::GetInstance().GetMaxFrameRate()),
		new UIButton("FPSMore", Vector2f(static_cast<float>(window->getSize().x / 1.7), static_cast<float>(_windowY / 1.45)), Color::White, Color::Cyan, "+", 50, FONT, SOUND_UP, _more),
		new UIButton("FPSLess", Vector2f(static_cast<float>(window->getSize().x / 2.5), static_cast<float>(_windowY / 1.45)), Color::White, Color::Cyan, "-", 50, FONT, SOUND_DOWN, _less),

		new UIButton("ReturnGraphics", Vector2f(_x, static_cast<float>(_windowY / 1.2)), Color::White, Color::Cyan, "Done", 40, FONT, SOUND_EXIT, _close) }, 3);
}

void MenuManager::GraphicMenu()
{
	Get("Graphics")->Toggle();
}

void MenuManager::InitLeaveLobby()
{
	float _x = static_cast<float>(window->getSize().x / 2);
	unsigned int _windowY = window->getSize().y;

	function<void()> _nextLevel = [&]() { LeaveLobby(); Map::GetInstance().NextMap(); };
	function<void()> _return = [this]() { LeaveLobby(); };

	new Menu("LeaveLobby", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)_windowY), AREYOUSURE),
		new UIText("AreYouSureText1", Vector2f(_x, static_cast<float>(_windowY / 4)), Color::White, "You have Diamonds !",35,FONT),
		new UIText("AreYouSureText2", Vector2f(_x, static_cast<float>(_windowY / 3.3)), Color::White, "Are you sure you want to leave the lobby?",35,FONT),
		new UIButton("StayText", Vector2f(_x, static_cast<float>(_windowY / 1.8)), Color::White, Color::Cyan, "No, spend Diamonds here", 35, FONT, SOUND_EXIT, _return),
		new UIButton("DeleteShit", Vector2f(_x, static_cast<float>(_windowY / 1.6)), Color::White, Color::Cyan, "Yes ! Go to fight !", 35, FONT, SOUND_START, _nextLevel) }, 4);
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
		new UIText("CalibText", Vector2f(_x, static_cast<float>(_windowY / 4)), Color::White, "Video/Audio Latency : ",35,FONT),
		new ProgressBar("CalibBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(_windowY / 2.72)), Vector2f(400.0f, 30.0f), EMPTYBAR, FULLBAR, MusicManager::GetInstance().GetAcceptDelay(), 450.0f),
		new UIButton("CalibUp", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(_windowY / 2.79)), Color::White, Color::Cyan, ">", 50, FONT, SOUND_UP, _calibrationUp),
		new UIButton("CalibDown", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(_windowY / 2.79)), Color::White, Color::Cyan, "<", 50, FONT, SOUND_DOWN, _calibrationDown),
		new UIText("CalibTextUpdate", Vector2f(_x, static_cast<float>(_windowY / 2)), Color(172, 172,173), "300",40,FONT, true),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(_x, static_cast<float>(_windowY / 1.2)), Color::White, Color::Cyan, "Done", 40, FONT, SOUND_EXIT, _close)
		}, 5);
}

void MenuManager::LatencyMenu()
{
	Get("LatencyMenu")->Toggle();
}
