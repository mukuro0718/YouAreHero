//==========================================
// @brief 攻撃用のスフィアColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class AttackData;
class AttackSphereColliderData : public ColliderData
{
public:
	AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);
	void OnHit();
	float radius;
	AttackData* data;
};
