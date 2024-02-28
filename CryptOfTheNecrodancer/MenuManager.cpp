#include "MenuManager.h"
#include "UIImage.h"
#include "UIText.h"
#include "ProgressBar.h"
#include "MusicManager.h"
#include "Soundmanager.h"

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

void MenuManager::InitMenu(RenderWindow* _window)
{
	window = _window;

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
			{ MenuManager::GetInstance().Get("GamePause")->Open(); MusicManager::GetInstance().Pause(); },
			{Event::KeyPressed, Keyboard::Escape}),
		ActionData("Select", [this]()
			{ MenuManager::GetInstance().ClickAction(); },
			{Event::MouseButtonPressed, Mouse::Left}) });

	function<void()> _callbackContinue = [this]() { Get("GamePause")->Toggle(); MusicManager::GetInstance().Unpause(); };
	function<void()> _callbackRestart = [this]() {}; //TODO
	function<void()> _callbackOptions = [this]() { Get("GamePause")->Toggle(); OptionsMenu(); };
	function<void()> _callbackLobby = [this]() {}; //TODO
	function<void()> _callbackDelete = [this]() { DeleteSaveDataMenu(); };
	function<void()> _callbackEchap = [this]() { Get("GamePause")->Toggle(); CloseMenu(); };

	new Menu("GamePause", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "PauseMenu.png"),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 5)),Color::White, Color::Cyan, "Continue Game", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackContinue),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.5)), Color::White, Color::Cyan, "Quick Restart", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackRestart),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.7)), Color::White, Color::Cyan, "Options", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackOptions),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.2)), Color::White, Color::Cyan, "Quit to Lobby", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _callbackLobby),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.85)), Color::White, Color::Cyan, "Delete Save Data", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _callbackDelete),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.3)), Color::White, Color::Cyan, "Exit Game", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _callbackEchap) }, 1);
}

void MenuManager::InitDeleteSaveDataMenu()
{
	function<void()> _delete = [&]() { Delete(); };
	function<void()> _return = [this]() { DeleteSaveDataMenu();  Get("GamePause")->Toggle(); };

	new Menu("Delete", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "AreYouSure.png"),
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4)), Color::White, "Are you sure you want to",35,"Assets/Font/Font.ttf"),
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color::White, "delete your save data ?",35,"Assets/Font/Font.ttf"),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _return),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes, delete this shit", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _delete) }, 2);
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
	//return to lobby
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

	new Menu("Options", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "OptionsMenu.png"),
		//Menu Graphique
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 4)), Color::White, Color::Cyan, "Graphical Options", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _graphics),

		// Activer/Désactiver le Sound
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.8)), Color(172, 172,173), "Sound",40,"Assets/Font/Font.ttf", true),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.8)), Color::White, Color(0,139,139), {
			new UIImage(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.8)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText(Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(window->getSize().y / 2.8)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _activateSound, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.8), 30.0f, 30.0f)),

		// Monter/Descendre le son du Sound
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.4)), Color(172, 172,173), "Sound Volume",40,"Assets/Font/Font.ttf", true),
		new ProgressBar(PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 2.12)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", SoundManager::GetInstance().GetVolume()),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 2.19)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _volumeUpS),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 2.19)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _volumeDownS),

		// Activer/Désactiver la musique
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.75)), Color(172, 172,173), "Music",40,"Assets/Font/Font.ttf", true),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75)), Color::White, Color(0,139,139), {
			new UIImage(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText(Vector2f(static_cast<float>(window->getSize().x / 1.425), static_cast<float>(window->getSize().y / 1.75)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _activateMusic, FloatRect(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.75), 30.0f, 30.0f)),

		// Monter/Descendre le son de la musique
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.55)), Color(172, 172,173), "Music Volume",40,"Assets/Font/Font.ttf", true),
		new ProgressBar(PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 1.4)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png", MusicManager::GetInstance().GetVolume()),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_up.ogg", _volumeUpM),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_select_down.ogg", _volumeDownM),

		// Retour menu précédent
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _close) }, 2);
}

void MenuManager::OptionsMenu()
{
	Get("Options")->Toggle();
}

void MenuManager::InitMenuClose()
{
	function<void()> _close = [&]() { window->close(); };
	function<void()> _return = [this]() { CloseMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("AreYouSure", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "AreYouSure.png"),
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color::White, "Are you sure you want to leave?",35,"Assets/Font/Font.ttf"),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _return),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes, quit the game", 35, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_start.ogg", _close) }, 2);
}

void MenuManager::CloseMenu()
{
	Get("AreYouSure")->Toggle();
}

void MenuManager::InitGraphicMenu()
{
	function<void()> _fullscreen = [this]() { /*Toggle FullScreen*/ };
	function<void()> _close = [this]() { OptionsMenu(); Get("Graphics")->Toggle(); };

	new Menu("Graphics", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "GraphicsMenu.png"),
		//Fullscreen on/off
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.3)), Color(172, 172,173), "Fullscreen",35,"Assets/Font/Font.ttf", true),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.6), static_cast<float>(window->getSize().y / 3.3)), Color::White, Color(0,139,139), {
			new UIImage(Vector2f(static_cast<float>(window->getSize().x / 1.6), static_cast<float>(window->getSize().y / 3.3)), Vector2f(30.0f, 30.0f), "EmptyCheckbox.png"),
			new UIText(Vector2f(static_cast<float>(window->getSize().x / 1.57), static_cast<float>(window->getSize().y / 3.3)), Color(0,139,139), "X", 40, "Assets/Font/Font.ttf", false)
		}, "Assets/Sounds/sfx_ui_toggle.ogg", _fullscreen, FloatRect(static_cast<float>(window->getSize().x / 1.6), static_cast<float>(window->getSize().y / 3.3), 30.0f, 30.0f)),

		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", "Assets/Sounds/sfx_ui_back.ogg", _close) }, 3);
}

void MenuManager::GraphicMenu()
{
	Get("Graphics")->Toggle();
}