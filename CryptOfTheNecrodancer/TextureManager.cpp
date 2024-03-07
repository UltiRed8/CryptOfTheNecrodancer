#include "TextureManager.h"
#include"Macro.h"
#include <iostream>

void TextureManager::Load(Shape* _shape, const string& _path, const bool _isRepeated, const bool _isSmooth)
{
	if (_path == "") return;

	TextureData* _textureData = Get(_path);

	if (!_textureData)
	{
		_textureData = new TextureData(_path);
		if (!_textureData->loadFromFile("Assets/Textures/"+_path))
		{
			cerr << "La texture n'a pas été correctement chargée !" << endl;
		}

		_textureData->setRepeated(_isRepeated);
		//_textureData->setSmooth(_isSmooth);

		_shape->setTexture(_textureData);
		_textureData->SetRect(_shape->getTextureRect());
	}
	else
	{
		_shape->setTexture(_textureData);
	}

	_shape->setTextureRect(_textureData->GetRect());
}

void TextureManager::LoadFromTextureSheet(Shape* _shape, const string& _path, int _textureIndex, const Vector2i& _texturePartSize)
{

	if (_path == "") return;

	TextureData* _textureData = Get(_path);

	if (!_textureData)
	{
		_textureData = new TextureData(_path);
		if (!_textureData->loadFromFile("Assets/Textures/" +_path))
		{
			cerr << "La texture n'a pas été correctement chargée !" << endl;
		}

		//_textureData->setSmooth(true);
	}

	const int _amountOfTexturesPerLines = _textureData->getSize().x / _texturePartSize.x;

	int _posX = 0;
	int _posY = 0;

	while (_textureIndex >= 1)
	{
		_textureIndex--;
		_posX++;
		if (_posX > _amountOfTexturesPerLines)
		{
			_posX = 0;
			_posY++;
		}
	}
	const Vector2i& _texturePosition = Vector2i(_posX * _texturePartSize.x, _posY * _texturePartSize.y);
	IntRect _textureRect = IntRect(_texturePosition, _texturePartSize);

	_shape->setTexture(_textureData);
	_shape->setTextureRect(_textureRect);
}