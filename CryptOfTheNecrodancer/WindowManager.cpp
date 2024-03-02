#include "WindowManager.h"
#include "Macro.h"

WindowManager::WindowManager()
{
	CreateWindow();
}

WindowManager::~WindowManager()
{
	delete window;
}

void WindowManager::CreateWindow()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer", Style::Close);
}

void WindowManager::Rename(const string& _newWindowName)
{
	if (_newWindowName.empty()) return;
	window->setTitle(_newWindowName);
}

void WindowManager::Shake(const int _strength)
{
	// TODO implement this
}