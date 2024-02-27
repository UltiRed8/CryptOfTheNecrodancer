#include "MenuManager.h"
#include "UIImage.h"
#include "UIText.h"
#include "ProgressBar.h"
#include "MusicManager.h"

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

	function<void()> _callbackContinue = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); MusicManager::GetInstance().Unpause(); };
	function<void()> _callbackRestart = [this]() {};
	function<void()> _callbackOptions = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); OptionsMenu(); };
	function<void()> _callbackLobby = [this]() {};
	function<void()> _callbackEchap = [this]() { MenuManager::GetInstance().Get("GamePause")->Toggle(); CloseMenu(); };

	new Menu("GamePause", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "PauseMenu.png"),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 5)),Color::White, Color::Cyan, "Continue Game", 50, "Assets/Font/Font.ttf", _callbackContinue),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 3.5)), Color::White, Color::Cyan, "Quick Restart", 50, "Assets/Font/Font.ttf", _callbackRestart),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.7)), Color::White, Color::Cyan, "Options", 50, "Assets/Font/Font.ttf", _callbackOptions),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2.2)), Color::White, Color::Cyan, "Quit to Lobby", 50, "Assets/Font/Font.ttf", _callbackLobby),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.3)), Color::White, Color::Cyan, "Exit Game", 50, "Assets/Font/Font.ttf", _callbackEchap) }, 1);
}

void MenuManager::InitMenuOptions()
{
	function<void()> _activate = [this]() { cout << "Sound Toggle" << endl; };
	function<void()> _volumeUp = [this]() { cout << "Volume Up" << endl; };
	function<void()> _volumeDown = [this]() { cout << "Volume Down" << endl; };
	function<void()> _close = [this]() { OptionsMenu(); MenuManager::GetInstance().Get("GamePause")->Toggle(); };

	new Menu("Options", { new UIImage(Vector2f(0.f,0.f), Vector2f((float)window->getSize().x, (float)window->getSize().y), "OptionsMenu.png"),
		// Activer/Désactiver la musique
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.73)), Color(172, 172,173), "Music",40,"Assets/Font/Font.ttf", true),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.73)), Color::White, Color::Cyan, "X", 40, "Assets/Font/Font.ttf", _activate),

		// Monter/Descendre le son de la musique
		new UIText(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.55)), Color(172, 172,173), "Music Volume",40,"Assets/Font/Font.ttf", true),
		new ProgressBar(PT_LEFT, Vector2f(static_cast<float>(window->getSize().x / 2.9), static_cast<float>(window->getSize().y / 1.4)), Vector2f(400.0f, 30.0f), "EmptyBar.png", "FullBar.png"),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 1.45), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, ">", 50, "Assets/Font/Font.ttf", _volumeUp),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 3.2), static_cast<float>(window->getSize().y / 1.43)), Color::White, Color::Cyan, "<", 50, "Assets/Font/Font.ttf", _volumeDown),

		// Retour menu précédent
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.2)), Color::White, Color::Cyan, "Done", 40, "Assets/Font/Font.ttf", _close) }, 2);
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
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.8)), Color::White, Color::Cyan, "No, stay here", 35, "Assets/Font/Font.ttf", _return),
		new UIButton(Vector2f(static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 1.6)), Color::White, Color::Cyan, "Yes, quit the game", 35, "Assets/Font/Font.ttf", _close) }, 2);
}

void MenuManager::CloseMenu()
{
	Get("AreYouSure")->Toggle();
}
