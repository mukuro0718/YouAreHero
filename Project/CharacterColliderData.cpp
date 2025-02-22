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
	: ColliderData		 (ColliderData::Kind::CHARACTER_CAPSULE, _priority, _tag)
	, radius			 (0.0f)
	, topPositon		 (VGet(0.0f, 0.0f, 0.0f))
	, data				 (_data)
	, isUseCollWithChara (false)
	, isUseCollWithGround(true)
	, isSetTopPosition	 (false)
	, type				 (CharaType::NONE)
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
	/*カードが成功しているか*/
	bool isGuard = true;
	if (this->data->isGuard)
	{
		this->data->stamina -= _data.blockStaminaConsumption;
		//残りのスタミナが0未満ならガード失敗
		if (this->data->stamina < 0)
		{
			this->data->stamina = 0;
			this->data->isGuard = false;
			isGuard = false;
		}
	}
	else
	{
		isGuard = false;
	}

	/*ガードも無敵フラグもたっていなかったら*/
	if (!this->data->isInvinvible && !isGuard)
	{
		//ダメージ計算
		this->data->damage		 = _data.damage / this->data->defensivePower;
		//HPを減らす
		this->data->hp			-= this->data->damage;
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

