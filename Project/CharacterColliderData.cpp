#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "CharacterColliderData.h"
#include "HitStopManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CharacterColliderData::CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data)
	: ColliderData	(ColliderData::Kind::CHARACTER_CAPSULE, _priority, _tag)
	, radius		(0.0f)
	, topPositon	(VGet(0.0f, 0.0f, 0.0f))
	, data			(_data)
{
	//処理なし
}

/// <summary>
/// キャラクターデータの取得
/// </summary>
const CharacterData& CharacterColliderData::GetCharacterData()const
{
	return *data;
}

/// <summary>
/// ヒット処理
/// </summary>
void CharacterColliderData::OnHit(const AttackData& _data, const VECTOR _attackPosition)
{
	auto& hitStop = Singleton<HitStopManager>::GetInstance();

	/*ガードも無敵フラグもたっていなかったら*/
	if (!this->data->isInvinvible && !this->data->isGuard)
	{
		//HPを減らす
		this->data->hp			 -= _data.damage;
		//ヒットストップの設定
		this->data->hitStopTime  = _data.hitStopTime;
		this->data->hitStopType  = _data.hitStopType;
		this->data->hitStopDelay = _data.hitStopDelay;
		this->data->slowFactor	 = _data.slowFactor;
		this->data->reactionType = _data.reactionType;
	}

	/*ヒットフラグを立てる*/
	this->data->isHit = true;
}

