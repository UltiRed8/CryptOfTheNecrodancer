#pragma once

#include "IManager.h"
#include "TextureData.h"
#include "Singleton.h"

class TextureManager : public Singleton<TextureManager>, public IManager<string, TextureData>
{

public:
	void Load(Shape* _shape, const string& _path, const bool _isRepeated = false, const bool _isSmooth = true);
	void LoadFromTextureSheet(Shape* _shape, const string& _path, int _textureIndex, const Vector2i& _texturePartSize);
};