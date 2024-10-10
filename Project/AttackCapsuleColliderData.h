//==========================================
// @brief 攻撃用のカプセルColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class AttackCapsuleColliderData : public ColliderData
{
public:
	AttackCapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);
	void OnHit(const CharacterData& _data);
	float radius;
	VECTOR topPositon;
	AttackData* data;
};
