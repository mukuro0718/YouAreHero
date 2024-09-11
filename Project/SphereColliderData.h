//==========================================
// @brief ‹…‚ÌColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class SphereColliderData : public ColliderData
{
public:
	SphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);
	float radius;
};
