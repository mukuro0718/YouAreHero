//==========================================
// @brief ƒJƒvƒZƒ‹‚ÌColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CapsuleColliderData : public ColliderData
{
public:
	CapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);
	float radius;
	VECTOR topPositon;
};
