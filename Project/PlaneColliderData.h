//==========================================
// @brief •½–Ê(¡‰ñ‚Í‰~Œ`‚É‚·‚é‚Ì‚Å”¼Œa‚à‚Â)‚ÌColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class PlaneColliderData : public ColliderData
{
public:
	PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);
	VECTOR norm;//–@ü
	float radius;
};
