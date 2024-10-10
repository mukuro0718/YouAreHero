#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"

AttackCapsuleColliderData::AttackCapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData(ColliderData::Kind::ATTACK_CAPSULE, _priority, _tag)
	, radius(0.0f)
	, topPositon(VGet(0.0f, 0.0f, 0.0f))
	, data(_data)
{
	//ˆ—‚È‚µ
}
void AttackCapsuleColliderData::OnHit(const CharacterData& _data)
{
	this->data->isDoHitCheck = false;
	if (!_data.isInvinvible && !_data.isGuard)
	{
		this->data->isHitAttack = true;
	}
}