#include "FontManager.h"

#include <iostream>

FontManager::FontManager()
{
	ResetAlpha();
}

void FontManager::Fade(const vector<Text*>& _texts, const string& _path, const Color& _targetColor)
{
	currentAlpha += alphaFactor;

	if (alphaFactor > 0 && currentAlpha >= 255)
	{
		fadeTimer->Destroy();
		ResetAlpha();
	}

	if (alphaFactor < 0 && currentAlpha <= 30)
	{
		for (Text* _text : _texts)
		{
			Load(_text, _path);
			_text->setFillColor(_targetColor);
		}
		alphaFactor = 1;
		currentAlpha = 0;
	}
	else
	{
		for (Text* _text : _texts)
		{
			Color _color = _text->getFillColor();
			_color.a = currentAlpha;
			_text->setFillColor(_color);
		}
	}
}

void FontManager::ResetAlpha()
{
	fadeTimer = nullptr;
	alphaFactor = -1;
	currentAlpha = 255;
}

void FontManager::Load(Text* _text, const string& _path, const bool _isSmooth)
{
	FontData* _fontData = Get(_path);

	if (!_fontData)
	{
		_fontData = new FontData(_path);
		if (!_fontData->loadFromFile(_path))
		{
			cerr << "La police d'écriture n'a pas été correctement chargée !" << endl;
		}

		_fontData->setSmooth(false);
	}

	_text->setFont(*_fontData);
}

void FontManager::LoadWithFade(const vector<Text*>& _texts, const string& _path, const float _duration, const Color& _targetColor)
{
	if (fadeTimer) return;

	alphaFactor = -1;
	const function<void()> _callback = [this, _texts, _path, _targetColor]() { Fade(_texts, _path, _targetColor); };
	fadeTimer = new Timer("FontFade", _callback, seconds(_duration / (255 * 2)), -1, false);
}
