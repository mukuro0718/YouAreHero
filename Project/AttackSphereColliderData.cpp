#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"

/// <summary>
/// コンストラクタ
/// </summary>
AttackSphereColliderData::AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData	(ColliderData::Kind::ATTACK_SPHERE, _priority, _tag)
	, radius		(0.0f)
	, data			(_data)
{
	//処理なし
}

/// <summary>
/// ヒット処理
/// </summary>
void AttackSphereColliderData::OnHit(const CharacterData& _data)
{
	//攻撃ヒットチェックフラグを下す
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
		//攻撃に当たったフラグを立てる
		this->data->isHitAttack = true;
	}
}