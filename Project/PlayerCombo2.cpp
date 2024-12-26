#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo2.h"
#include "EffectManager.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerCombo2::PlayerCombo2()
	: PlayerAction()
{
	/*コライダーデータの作成*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, new AttackData());

	/*初期化*/
	auto& json						  = Singleton<JsonManager>::GetInstance();
	this->hitStopTime				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_TIME"];
	this->hitStopType				  = static_cast<int>(HitStop::Type::SLOW);
	this->hitStopDelay				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_DELAY"];
	this->slowFactor				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_FACTOR"];
	this->collider->radius			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_ATTACK_RADIUS"];					//半径
	this->collider->data->hitStopTime = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_HIT_STOP_TIME"];					//ヒットストップ時間
	this->collider->data->damage	  = json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][1];						//ダメージ
	this->nextAnimation				  = static_cast<int>(Player::AnimationType::COMBO_2);										//次のアニメーション
	this->playTime					  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];//アニメーション再生時間
	this->firstPlayTime				  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_ANIM_PLAY_TIME"];				//アニメーションの初期再生時間
	this->cancelableFrame			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_CANCEL_FRAME"];						//キャンセル可能フレーム
	this->startHitCheckFrame		  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_HIT_CHECK_FRAME"];			//当たり判定開始フレーム
	this->endHitCheckFrame			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_END_HIT_CHECK_FRAME"];				//当たり判定終了フレーム
	this->positionOffset			  = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"];							//座標オフセット値フレーム
	this->yOffset					  = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"];							//Yオフセット値フレーム
	this->firstDirection			  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);			//最初の向き
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerCombo2::~PlayerCombo2()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerCombo2::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction	 = false;
	this->frameCount	 = 0;
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));

}

/// <summary>
/// 後処理
/// </summary>
void PlayerCombo2::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerCombo2::Update(Player& _player)
{
	/*移動速度が０以上の時処理を行う*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	/*アニメーションの再生*/
	float playTime = this->playTime;
	if (this->frameCount == 0) playTime = this->firstPlayTime;
	_player.PlayAnimation(this->nextAnimation, playTime);

	/*キャンセルフラグが立っていなったら*/
	this->frameCount++;
	if (!this->isChangeAction)
	{
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
		}
	}

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}

	/*当たり判定が開始していなければ早期リターン*/
	if (this->frameCount < this->startHitCheckFrame) return;

	/*当たり判定が開始したタイミングで1度だけ呼ばれる*/
	if (this->frameCount == this->startHitCheckFrame)
	{
		this->collider->data->isDoHitCheck = true;
	}


	/*攻撃が当たっていたらエフェクトを再生*/
	if (this->collider->data->isHitAttack)
	{
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_IMPACT);
		effect.SetPosition(EffectManager::EffectType::PLAYER_IMPACT, this->collider->rigidbody.GetPosition());
		this->collider->data->isHitAttack = false;
		_player.SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
	}

	/*当たり判定許可フラグが立っていなかったら早期リターン*/
	if (!this->collider->data->isDoHitCheck) return;

	/*当たり判定座標の更新*/
	VECTOR direction = VTransform(this->firstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));	//向きの設定
	VECTOR position	 = _player.GetRigidbody().GetPosition();																//プレイヤーの座標
	position		 = VAdd(position, VScale(direction, this->positionOffset));								//プレイヤーの座標に、オフセット値を足す
	position.y		 += this->yOffset;																						//Y座標オフセット値を足す
	this->collider->rigidbody.SetPosition(position);

	//フレームが定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
	if (this->frameCount > this->endHitCheckFrame)
	{
		this->collider->data->isDoHitCheck = false;
	}

#ifdef _DEBUG
	DrawSphere3D(this->collider->rigidbody.GetPosition(), this->collider->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
#endif // _DEBUG

}