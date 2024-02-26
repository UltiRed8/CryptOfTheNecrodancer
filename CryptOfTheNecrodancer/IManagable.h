#pragma once

class TextureManager;
class EntityManager;
class MapManager;

template<typename ID>
class IManagable
{
	bool toRemove;
protected:
	ID id;

public:
	void SetID(const ID& _id)
	{
		id = _id;
	}
	ID GetID() const
	{
		return id;
	}
	bool IsToRemove() const
	{
		return toRemove;
	}
	void Destroy()
	{
		toRemove = true;
	}
	void SetToRemove()
	{
		toRemove = true;
	}

public:
	IManagable(const ID& _id)
	{
		id = _id;
		toRemove = false;
	}

private:
	virtual void Register() = 0;
};