//==========================================
// @brief 攻撃用のスフィアColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class AttackData;
class CharacterData;
class AttackSphereColliderData : public ColliderData
{
public:
	AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);
	void OnHit(const CharacterData& _data);
	float radius;
	AttackData* data;
};
