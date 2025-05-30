#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackCapsuleColliderData.h"

/// <summary>
/// コンストラクタ
/// </summary>
AttackCapsuleColliderData::AttackCapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData	(ColliderData::Kind::ATTACK_CAPSULE, _priority, _tag)
	, radius		(0.0f)
	, topPositon	(VGet(0.0f, 0.0f, 0.0f))
	, data			(_data)
{
	//処理なし
}

/// <summary>
/// ヒット処理
/// </summary>
void AttackCapsuleColliderData::OnHit(const CharacterData& _data)
{
	/*ヒットチェックフラグを下す*/
	this->data->isDoHitCheck = false;

	/*カードが成功しているか*/
	bool isGuard = false;
	if (_data.isGuard)
	{
		//残りのスタミナが足りていたらガード成功
		if (_data.stamina >= this->data->blockStaminaConsumption)
		{
			isGuard = true;
		}
	}

	/*無敵フラグもガードもしていなかったら*/
	if (!_data.isInvinvible && !isGuard)
	{
		//攻撃ヒットフラグを立てる
		this->data->isHitAttack = true;
	}
}