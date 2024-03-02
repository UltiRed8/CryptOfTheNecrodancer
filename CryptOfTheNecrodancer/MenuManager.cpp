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
		new UIImage("RythmHearts", Vector2f(SCREEN_WIDTH / 2 - 20 * 2, SCREEN_HEIGHT - 55 * 2), Vector2f(40.0f, 50.0f) * 2.0f, "RythmHearts0.png"),

		//Multiplier
		new UIText("CoinMultText", Vector2f(SCREEN_WIDTH / 2 - 20 * 0.5, SCREEN_HEIGHT - 55 * 0.2), Color::White, "Coin Multiplier: " , 15,"Assets/Font/Font.ttf", false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetChainMultiplier()),

		//Coins
		new UIImage("Coin", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 12.8), Vector2f(25.0f, 25.0f) * 2.0f, "Coin.png"),
		new UIText("CoinUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 12.5), Color::White, "x " ,25,"Assets/Font/Font.ttf", false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetMoney()),

		//Diamond
		new UIImage("Diamond", Vector2f(SCREEN_WIDTH - 55 * 3, SCREEN_HEIGHT - 55 * 11.6), Vector2f(25.0f, 25.0f) * 2.0f, "Diamond.png"),
		new UIText("DiamondUpdateText", Vector2f(SCREEN_WIDTH - 55 * 1.2, SCREEN_HEIGHT - 55 * 11.3), Color::White, "x " ,25,"Assets/Font/Font.ttf", false, dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetRessources()->GetDiamonds()),
		});

	_hud->Open();
}

void MenuManager::InitInventory()
{
	
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

	new Menu("GamePause", {
		new UIButton("ContinueButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 5)),Color::White, Color::Cyan, "Continue Game", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackContinue),
		new UIButton("RestartButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.5)), Color::White, Color::Cyan, "Quick Restart", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackRestart),
		new UIButton("OptionsButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.7)), Color::White, Color::Cyan, "Options", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackOptions),
		new UIButton("ReturnLobbyButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.2)), Color::White, Color::Cyan, "Quit to Lobby", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _callbackLobby),
		new UIButton("DeleteSaveDataButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.85)), Color::White, Color::Cyan, "Delete Save Data", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackDelete),
		new UIButton("Calibration", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.58)), Color::White, Color::Cyan, "Calibration Latency", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackCalibration),
		new UIButton("Exit", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.3)), Color::White, Color::Cyan, "Exit Game", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _callbackEchap),
		new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "PauseMenu.png"),}, 1);
}

void MenuManager::InitDeleteSaveDataMenu()
{
	function<void()> _delete = [&]() { Delete(); };
	function<void()> _return = [this]() { DeleteSaveDataMenu(); };

	new Menu("Delete", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "AreYouSure.png"),
		new UIText("AreYouSureText1", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4)), Color::White, "Are you sure you want to",35,"Assets/Font/Font.ttf"),
		new UIText("AreYouSureText2", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color::White, "delete your save data ?",35,"Assets/Font/Font.ttf"),
		new UIButton("StayText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _return),
		new UIButton("DeleteShit", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes, delete this shit", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _delete) }, 2);
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
	Map::GetInstance().Load("Assets/Saved/Lobby.txt");
}

void MenuManager::InitMenuOptions()
{
	function<void()> _activateSound = [this]() { SoundManager::GetInstance().ToggleVolume(); }; 
	function<void()> _activateMusic = [this]() { MusicManager::GetInstance().ToggleVolume(); }; 
	function<void()> _volumeUpM = [this]() { MusicManager::GetInstance().IncreaseVolume(); };
	function<void()> _volumeUpS = [this]() { SoundManager::GetInstance().IncreaseVolume(); }; 
	function<void()> _volumeDownM = [this]() { MusicManager::GetInstance().DecreaseVolume(); };
	function<void()> _volumeDownS = [this]() { SoundManager::GetInstance().DecreaseVolume(); }; 
	function<void()> _close = [this]() { OptionsMenu(); Get("GamePause")->Toggle(); };
	function<void()> _graphics = [this]() { GraphicMenu();  Get("Options")->Toggle(); };

	new Menu("Options", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "OptionsMenu.png"),
		//Menu Graphique
		new UIButton("GraphicalOptionsButton", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4.2)), Color::White, Color::Cyan, "Graphical Options", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _graphics),

		// Activer/Désactiver le Sound
		new UIText("ToggleSText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3)), Color(172, 172,173), "Toggle Sound",40,"Assets/Font/Font.ttf", true),
		new UIButton("CheckBoxS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 3)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageS", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 3)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText("CheckBoxTextS", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(window->getSize().y / 3)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _activateSound, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 3), 30.0f, 30.0f)),

		// Monter/Descendre le son du Sound
		new UIText("SoundVolText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.4)), Color(172, 172,173), "Sound Volume",40,"Assets/Font/Font.ttf", true),
		new ProgressBar("SoundBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 2.12)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", SoundManager::GetInstance().GetVolume()),
		new UIButton("SoundMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.19)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _volumeUpS),
		new UIButton("SoundLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 2.19)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _volumeDownS),

		// Activer/Désactiver la musique
		new UIText("ToggleMText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.75)), Color(172, 172,173), "Toggle Music",40,"Assets/Font/Font.ttf", true),
		new UIButton("CheckBoxM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText("CheckBoxImageM", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(window->getSize().y / 1.75)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _activateMusic, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75), 30.0f, 30.0f)),

		// Monter/Descendre le son de la musique
		new UIText("MusicVolText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.52)), Color(172, 172,173), "Music Volume",40,"Assets/Font/Font.ttf", true),
		new ProgressBar("MusicBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 1.4)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", MusicManager::GetInstance().GetVolume()),
		new UIButton("MusicMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _volumeUpM),
		new UIButton("MusicLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _volumeDownM),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _close) }, 2);
}

void MenuManager::OptionsMenu()
{
	Get("Options")->Toggle();
}

void MenuManager::InitMenuClose()
{
	function<void()> _close = [&]() { window->close(); };
	function<void()> _return = [this]() { CloseMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("AreYouSure", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "AreYouSure.png"),
		new UIText("AreYouSureTextClose", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color::White, "Are you sure you want to leave?",35,"Assets/Font/Font.ttf"),
		new UIButton("StayAreYouSure", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _return),
		new UIButton("LeaveGame", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes, quit the game", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _close) }, 2);
}

void MenuManager::CloseMenu()
{
	Get("AreYouSure")->Toggle();
}

void MenuManager::InitGraphicMenu()
{
	Vector2f _index;
	function<void()> _up = [this]() { CameraManager::GetInstance().ZoomOut(); };
	function<void()> _down = [this]() { CameraManager::GetInstance().ZoomIn(); };

	function<void()> _more = [this]() { TimerManager::GetInstance().UpdateFrameRate(10); };
	function<void()> _less = [this]() { TimerManager::GetInstance().UpdateFrameRate(-10); };

	function<void()> _resetZoom = [this]() { CameraManager::GetInstance().Reset(); };

	function<void()> _close = [this]() { OptionsMenu(); Get("Graphics")->Toggle(); };

	new Menu("Graphics", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "GraphicsMenu.png"),
		//View Multiplier
		new UIText("ViewText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.2)), Color(172, 172,173), "View Multiplier",40,"Assets/Font/Font.ttf", true),
		new ProgressBar("ViewBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 2.72)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", CameraManager::GetInstance().GetZoomIndex(), 1.5f),
		new UIButton("ViewMore", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.79)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _up),
		new UIButton("ViewLess", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 2.79)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _down),
		
		// Activer/Désactiver le Sound
		new UIText("ToggleZText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2)), Color(172, 172,173), "Zoom per default",40,"Assets/Font/Font.ttf", true),
		new UIButton("CheckBoxZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2)), Color::White, Color(0,139,139), {
			new UIImage("CheckBoxImageZ", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText("CheckBoxTextZ", Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(window->getSize().y / 2)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _resetZoom, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2), 30.0f, 30.0f)),

		//SetFrameRateLimit
		new UIText("FPS", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color(172, 172,173), "Framerate Limit",40,"Assets/Font/Font.ttf", true),
		new UIText("FPSTextUpdate", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.4)), Color(172, 172,173), "",40,"Assets/Font/Font.ttf", true, TimerManager::GetInstance().GetMaxFrameRate()),
		new UIButton("FPSMore", Vector2f(static_cast<float>(window->getSize().x / 1.7), static_cast<float>(window->getSize().y / 1.45)), Color::White, Color::Cyan, "+", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _more),
		new UIButton("FPSLess", Vector2f(static_cast<float>(window->getSize().x / 2.5), static_cast<float>(window->getSize().y / 1.45)), Color::White, Color::Cyan, "-", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _less),

		new UIButton("ReturnGraphics", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _close) }, 3);
}

void MenuManager::GraphicMenu()
{
	Get("Graphics")->Toggle();
}

void MenuManager::InitLeaveLobby()
{
	function<void()> _nextLevel = [&]() { LeaveLobby(); Map::GetInstance().NextMap(); };
	function<void()> _return = [this]() { LeaveLobby(); };

	new Menu("LeaveLobby", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "AreYouSure.png"),
		new UIText("AreYouSureText1", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4)), Color::White, "You have Diamonds !",35,"Assets/Font/Font.ttf"),
		new UIText("AreYouSureText2", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color::White, "Are you sure you want to leave the lobby?",35,"Assets/Font/Font.ttf"),
		new UIButton("StayText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, spend Diamonds here", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _return),
		new UIButton("DeleteShit", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes ! Go to fight !", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _nextLevel) }, 4);
}

void MenuManager::LeaveLobby()
{
	Get("LeaveLobby")->Toggle();
}

void MenuManager::InitMenuLatency()
{
	function<void()> _calibrationUp = [this]() { MusicManager::GetInstance().SetAcceptDelay(10.0f); dynamic_cast<UIText*>(Get("LatencyMenu")->Get("CalibTextUpdate"))->GetText()->setString(to_string(*MusicManager::GetInstance().GetAcceptDelay()).substr(0, to_string(*MusicManager::GetInstance().GetAcceptDelay()).find_first_of('.'))); }; //TODO
	function<void()> _calibrationDown = [this]() { MusicManager::GetInstance().SetAcceptDelay(-10.0f); dynamic_cast<UIText*>(Get("LatencyMenu")->Get("CalibTextUpdate"))->GetText()->setString(to_string(*MusicManager::GetInstance().GetAcceptDelay()).substr(0, to_string(*MusicManager::GetInstance().GetAcceptDelay()).find_first_of('.'))); }; //TODO
	function<void()> _close = [this]() { LatencyMenu(); Get("GamePause")->Toggle(); };

	new Menu("LatencyMenu", { new UIImage("1", Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "LatencyMenu.png"), 
		new UIText("CalibText", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4)), Color::White, "Video/Audio Latency : ",35,"Assets/Font/Font.ttf"),
		new ProgressBar("CalibBar", PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 2.72)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", MusicManager::GetInstance().GetAcceptDelay(), 450.0f),
		new UIButton("CalibUp", Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.79)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _calibrationUp),
		new UIButton("CalibDown", Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 2.79)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _calibrationDown),
		new UIText("CalibTextUpdate", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2)), Color(172, 172,173), "300",40,"Assets/Font/Font.ttf", true),

		// Retour menu précédent
		new UIButton("ReturnOptions", Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _close)
		}, 5);
}

void MenuManager::LatencyMenu()
{
	Get("LatencyMenu")->Toggle();
}
