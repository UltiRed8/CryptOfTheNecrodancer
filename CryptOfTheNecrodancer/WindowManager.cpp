#include "WindowManager.h"
#include "Macro.h"
#include "TimerManager.h"
#include "InputManager.h"
#include "CameraManager.h"

WindowManager::WindowManager()
{
	window = nullptr;
	shader = nullptr;
	isShakable = new bool(true);
	Init();
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

void WindowManager::CreateWindow(const bool _fullscreen)
{
	int _style = Style::Close;
	_style = _fullscreen ? _style | Style::Fullscreen : _style;
	fullscreen = _fullscreen;
	if (window)
	{
		window->create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer", _style);
	}
	else
	{
		window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Crypt of the Necrodancer", _style);
	}
	baseWindowPosition = window->getPosition();
	currentPosition = Vector2f(baseWindowPosition);
}

void WindowManager::Init()
{
	CreateWindow(false);
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

void WindowManager::Shake(const Vector2f& _direction)
{
	if (!*isShakable) return;

	if (IsInFullscreen())
	{
		CameraManager::GetInstance().Shake(_direction);
		return;
	}

	if (_direction == Vector2f(0.0f, 0.0f))
	{
		const int _rotation = Random(360, 0);

		const float _dirX = (const float) cos(_rotation);
		const float _dirY = (const float) sin(_rotation);

		direction = Vector2f(_dirX, _dirY);
	}
	else
	{
		direction = _direction;
	}

	const Vector2f& _offset = direction * 50.0f;

	currentPosition = Vector2f(baseWindowPosition) + _offset;
	window->setPosition(Vector2i(currentPosition));
}

void WindowManager::Update()
{
	if (!*isShakable) return;

	if (!IsNearlyEquals(currentPosition, Vector2f(baseWindowPosition), 10.0f))
	{
		if (currentPosition.x != baseWindowPosition.x)
		{
			if ((direction.x > 0.0f && currentPosition.x < baseWindowPosition.x) || (direction.x < 0.0f && currentPosition.x > baseWindowPosition.x))
			{
				currentPosition.x = (const float) baseWindowPosition.x;
			}
		}
		if (currentPosition.y != baseWindowPosition.y)
		{
			if ((direction.y > 0.0f && currentPosition.y < baseWindowPosition.y) || (direction.y < 0.0f && currentPosition.y > baseWindowPosition.y))
			{
				currentPosition.y = (const float) baseWindowPosition.y;
			}
		}

		const float _delta = TimerManager::GetInstance().GetDeltaTime();
		if (currentPosition.x != baseWindowPosition.x) currentPosition.x += currentPosition.x - baseWindowPosition.x > 0 ? -_delta : _delta;
		if (currentPosition.y != baseWindowPosition.y) currentPosition.y += currentPosition.y - baseWindowPosition.y > 0 ? -_delta : _delta;
		window->setPosition(Vector2i(currentPosition));
	}
} 