#pragma once
#include "Enemy.h"

#define PATH_SKELETON "SkeletonSprite.png"
#define PATH_YELLOW_SKELETON "empty"
#define PATH_BLACK_SKELETON "empty"

enum SkeletonType
{
	SK_NONE, SK_NORMAL, SK_YELLOW , SK_BLACK
};

class Skeleton : public Enemy
{
protected:
	SkeletonType skeletonType;
public:
	Skeleton(const Vector2f& _position, const string& _path, const int _droppedCoins);

public:
	void SelectDirection();
	void UpdateRythm();
};

struct NormalSkeleton : public Skeleton
{
	NormalSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_SKELETON, 2)
	{
		skeletonType = SK_NORMAL;
	}
};

struct YellowSkeleton : public Skeleton
{
	YellowSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_YELLOW_SKELETON, 3)
	{
		skeletonType = SK_YELLOW;
	}
};

struct BlackSkeleton : public Skeleton
{
	BlackSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_BLACK_SKELETON, 4)
	{
		skeletonType = SK_BLACK;
	}
};