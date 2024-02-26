#pragma once

#include "IManager.h"
#include "FontData.h"
#include "Singleton.h"
#include "Timer.h"

class FontManager : public Singleton<FontManager>, public IManager<string, FontData>
{
	Timer* fadeTimer;
	int currentAlpha;
	int alphaFactor;

public:
	FontManager();

private:
	void Fade(const vector<Text*>& _texts, const string& _path, const Color& _targetColor);
	void ResetAlpha();

public:
	void Load(Text* _text, const string& _path, const bool _isSmooth = true);
	void LoadWithFade(const vector<Text*>& _texts, const string& _path, const float _duration = 2.0f, const Color& _targetColor = Color::White);
};