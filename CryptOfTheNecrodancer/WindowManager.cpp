#include "WindowManager.h"
#include "Macro.h"
#include "TimerManager.h"
#include "InputManager.h"

WindowManager::WindowManager()
{
	shader = nullptr;
	isShakable = new bool(true);
	CreateWindow();
	currentShader = 0;
}

WindowManager::~WindowManager()
{
	delete window;
	if (shader)
	{
		delete shader;
	}
}

void WindowManager::CreateWindow()
{
	window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer", Style::Close);
	baseWindowPosition = window->getPosition();
	Init();
}

void WindowManager::Init()
{
	InitShaders();
}

void WindowManager::InitShaders()
{
	if (!Shader::isAvailable()) return;

	new ActionMap("ShaderSwitching", {
		ActionData("Shader", [this]() { currentShader++; if (currentShader >= shaderNames.size()) currentShader = 0; LoadShader(currentShader); }, {Event::MouseButtonPressed, Mouse::Middle}),
	});

	shader = new Shader();
	shaderNames = {
		"BlackWhite.frag",
		"ColoredSpiralShader.frag",
		"ColorShader.frag",
		"GhostShader.frag",
		"NegativeShader.frag",
		"RainbowShader.frag",
		"WavesShader.frag",
	};

	return;

	/*Vector2f _lightPositions[3];
	Vector3f _lightColors[3];
	float _lightIntensities[3];*/

	if (!shader->loadFromFile("Assets/PlayerLights.frag", Shader::Fragment))
	{
		cerr << "Errow while loading fragment shader!" << endl;
		return;
	}

	/*_lightPositions[0] = Vector2f(140.0f, 360.0f);
	_lightColors[0] = Vector3f(0.0f, 0.0f, 1.0f);
	_lightIntensities[0] = 300.0f;
	_lightPositions[1] = Vector2f(640.0f, 360.0f);
	_lightColors[1] = Vector3f(1.0f, 1.0f, 1.0f);
	_lightIntensities[1] = 200.0f;
	_lightPositions[2] = Vector2f(1140.0f, 360.0f);
	_lightColors[2] = Vector3f(1.0f, 0.0f, 0.0f);
	_lightIntensities[2] = 100.0f;*/
	shader->setUniform("lightPosition", Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT /2.0f));
	/*shader->setUniformArray("lightColors", _lightColors, 3);
	shader->setUniformArray("lightRanges", _lightIntensities, 3);*/
}

void WindowManager::LoadShader(const int _id)
{
	if (!shader) return;
	if (_id < 0 || _id >= (const int)(shaderNames.size())) return;
	if (_id == 0)
	{
		currentShader = 0;
		delete shader;
		shader = new Shader();
		return;
	}
	if (!shader->loadFromFile("Assets/Shaders/" + shaderNames[_id - 1] , Shader::Fragment))
	{
		cerr << "Errow while loading fragment shader! (" + shaderNames[_id - 1] + ")" << endl;
		return;
	}
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