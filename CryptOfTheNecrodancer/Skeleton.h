#pragma once
#include "Enemy.h"

#define PATH_SKELETON "SkeletonSprite2.png"
#define PATH_YELLOW_SKELETON "Yellow_Skeleton.png"
#define PATH_BLACK_SKELETON "Black_Skeleton.png"

enum SkeletonType
{
	SK_NONE, SK_NORMAL, SK_YELLOW , SK_BLACK
};

class Skeleton : public Enemy
{
protected:
	SkeletonType skeletonType;
public:
	Skeleton(const Vector2f& _position, const string& _path);

public:
	void DieEvent() override;
	void SelectDirection();
	void UpdateRythm();
};

struct NormalSkeleton : public Skeleton
{
	NormalSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_SKELETON)
	{
		skeletonType = SK_NORMAL;
	}
};

struct YellowSkeleton : public Skeleton
{
	YellowSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_YELLOW_SKELETON)
	{
		skeletonType = SK_YELLOW;
	}
};

struct BlackSkeleton : public Skeleton
{
	BlackSkeleton(const Vector2f& _position) : Skeleton(_position, PATH_BLACK_SKELETON)
	{
		skeletonType = SK_BLACK;
	}
};