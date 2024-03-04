#pragma once
#include "Enemy.h"

#define PATH_SKELETON "Entities/SkeletonSprite.png"
#define PATH_YELLOW_SKELETON "Entities/empty"
#define PATH_BLACK_SKELETON "Entities/empty"

enum SkeletonType
{
	SK_NONE, SK_NORMAL, SK_YELLOW , SK_BLACK
};

class Skeleton : public Enemy
{

protected:
	SkeletonType skeletonType;
public:
	Skeleton(const float _maxHp,const float _maxDammage,const Vector2f& _position, const string& _path, const int _droppedCoins);

public:
	void SelectDirection();
	virtual void Update() override;
};

struct NormalSkeleton : public Skeleton
{
	NormalSkeleton(const Vector2f& _position) : Skeleton(100.f,50.f,_position, PATH_SKELETON, 2)
	{
		skeletonType = SK_NORMAL;
	}
};

struct YellowSkeleton : public Skeleton
{
	YellowSkeleton(const Vector2f& _position) : Skeleton(200.f,150.f,_position, PATH_YELLOW_SKELETON, 3)
	{
		skeletonType = SK_YELLOW;
	}
};

struct BlackSkeleton : public Skeleton
{
	BlackSkeleton(const Vector2f& _position) : Skeleton(300.f,200.f,_position, PATH_BLACK_SKELETON, 4)
	{
		skeletonType = SK_BLACK;
	}
};