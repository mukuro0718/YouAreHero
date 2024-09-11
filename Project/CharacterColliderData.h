//==========================================
// @brief ダメージ用カプセルColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class CharacterColliderData : public ColliderData
{
public:
	CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data);
	const CharacterData& GetCharacterData()const;
	void OnHit(const AttackData& _data, const VECTOR _attackPosition);
	float radius;
	VECTOR topPositon;
	CharacterData* data;
};
