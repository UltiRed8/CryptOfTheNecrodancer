#include "WindowManager.h"
#include "Macro.h"
#include "TimerManager.h"


WindowManager::WindowManager()
{
	isShakable = new bool(true);
	CreateWindow();
}

WindowManager::~WindowManager()
{
	delete window;
}

void WindowManager::CreateWindow()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer", Style::Close);
	baseWindowPosition = window->getPosition();
}

void WindowManager::Rename(const string& _newWindowName)
{
	if (_newWindowName.empty()) return;
	window->setTitle(_newWindowName);
}

void WindowManager::Shake(const int _strength)
{
	if (!*isShakable) return;
	currentValue = _strength;
	// TODO implement this
}

void WindowManager::Update()
{
	if (!*isShakable) return;

	if (currentValue > 0)
	{
		const int _dirX = Random(2, 0) - 1;
		const int _dirY = Random(2, 0) - 1;
		window->setPosition(baseWindowPosition + (Vector2i(_dirX, _dirY) * currentValue));
		currentValue--;
	}
} 