#pragma once

#include "Entity.h"
#include "Macro.h"
#include "FontManager.h"
#include "Zones.h"

#define FONT "Assets/Font/Font.ttf"

enum LockType
{
	LT_NONE, LT_FORCE, LT_BOSS
};

class Stair : public Entity
{
	LockType lock;
	string textValue;
	Text* text;
	Zone zoneToLoad;
	
public:
	void SetZoneToLoad(const Zone& _zone)
	{
		zoneToLoad = _zone;
	}
	bool IsLocked() const
	{
		return lock != LT_NONE;
	}
	void SetLocked(const LockType _lockType)
	{
		lock = _lockType;
		ChangeTexture();
	}
	void SetText(const string& _text)
	{
		text = new Text();
		text->setString(_text);
		FontManager::GetInstance().Load(text, FONT);
		text->setCharacterSize(15);
		SetOriginCentered(text);
		text->setPosition(shape->getPosition() + Vector2f(0.5f, -0.3f) * TILE_SIZE);
	}

public:
	Stair(const Vector2f& _position, const Zone& _zoneToLoad = Z_LOBBY, const LockType& _lockType = LT_NONE, const string& _textValue = "");
	~Stair();

private:
	void ChangeTexture();

public:
	bool OpenZone();
	virtual vector<Drawable*> GetDrawables() override
	{
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		if (text)
		{
			_drawables.push_back(text);
		}
		return _drawables;
	}
};