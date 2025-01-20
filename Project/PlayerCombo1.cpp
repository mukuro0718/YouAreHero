#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo1.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerCombo1::PlayerCombo1()
	: PlayerAction				()
	, FIRST_PLAY_TIME			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_START_ANIM_PLAY_TIME"])
	, CANCELABLE_PLAY_TIME		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_CANCEL_PLAY_TIME"])
	, START_HIT_CHECK_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_START_HIT_CHECK_PLAY_TIME"])
	, END_HIT_CHECK_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_END_HIT_CHECK_PLAY_TIME"])
	, POSITION_OFFSET			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET"])
	, Y_OFFSET					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"])
	, HIT_STOP_TIME				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_TIME"])
	, HIT_STOP_DELAY			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_DELAY"])
	, HIT_STOP_TYPE				(static_cast<int>(HitStop::Type::SLOW))
	, SLOW_FACTOR				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_FACTOR"])
	, STAMINA_CONSUMPTION		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["COMBO1_STAMINA_CONSUMPTION"])
	, firstDirection			(Gori::ORIGIN)
	, isStartHitCheck			(false)
	, collider					(nullptr)
{
	/*コライダーデータの作成*/
	this->collider = new AttackSphereColliderData(ColliderData::Priority::STATIC, GameObjectTag::PLAYER_ATTACK, new AttackData());

	/*初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	this->collider->radius			  = json.GetJson(JsonManager::FileType::PLAYER)["COMBO1_ATTACK_RADIUS"];
	this->collider->data->hitStopTime = json.GetJson(JsonManager::FileType::PLAYER)["COMBO1_HIT_STOP_TIME"];
	this->collider->data->damage	  = json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][0];	
	this->nextAnimation				  = static_cast<int>(Player::AnimationType::COMBO_1);
	this->playTime					  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->firstDirection			  = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
	this->totalPlayTime				  = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_TOTAL_PLAY_TIME"][this->nextAnimation];
	this->maxStamina				  = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerCombo1::~PlayerCombo1()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerCombo1::Initialize()
{
	this->isChangeAction	= false;
	this->isEndAction		= false;
	this->frameCount		= 0;
	this->nowTotalPlayTime	= 0;
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(VGet(0.0f, 500.0f, 0.0f));
}

/// <summary>
/// 後処理
/// </summary>
void PlayerCombo1::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerCombo1::Update(Player& _player)
{
	/*回転処理*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	if (this->frameCount < 10)
	{
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		VECTOR	nextRotation	= Gori::ORIGIN;
		VECTOR	enemyPosition	= enemy.GetRigidbody().GetPosition();
		VECTOR	positionToEnemy = VSub(_player.GetRigidbody().GetPosition(), enemyPosition);
				nextRotation.y	= static_cast<float>(atan2(static_cast<double>(positionToEnemy.x), static_cast<double>(positionToEnemy.z)));
				nowRotation		= Gori::LerpAngle(nowRotation, nextRotation, this->rotateLerpValue);
		_player.SetRotation(nowRotation, nextRotation);

	}

	/*移動速度の更新*/
	_player.SetSpeed(0.0f);

	/*移動ベクトルを出す*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	float playTime = this->playTime;
	if (this->nowTotalPlayTime == 0)
	{
		//スタミナを消費させる
		_player.CalcStamina(this->STAMINA_CONSUMPTION, this->maxStamina);
		//最初だけ再生時間を変更する
		playTime = this->FIRST_PLAY_TIME;
	}
	this->nowTotalPlayTime += playTime;
	_player.PlayAnimation(this->nextAnimation, playTime);

	/*キャンセルフラグが立っていないときにキャンセル可能までアニメーションが進んでいたらキャンセルフラグを立てる*/
	if (!this->isChangeAction)
	{
		if (this->nowTotalPlayTime >= this->CANCELABLE_PLAY_TIME)
		{
			this->isChangeAction = true;
			this->isStartHitCheck = false;
		}
	}

	/*アニメーションが終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}

	/*当たり判定開始以上かつ終了未満内までアニメーションが進んでいなければ早期リターン*/
	if (this->nowTotalPlayTime < this->START_HIT_CHECK_PLAY_TIME || this->nowTotalPlayTime > this->END_HIT_CHECK_PLAY_TIME) return;

	/*あたり判定が開始しているときに、当たり判定フラグが一度もたっていなかったらフラグを立てる*/
	if (!this->isStartHitCheck)
	{
		this->collider->data->isDoHitCheck = true;
		this->isStartHitCheck = true;
	}

	/*攻撃が当たっていたらエフェクトを再生*/
	if (this->collider->data->isHitAttack)
	{
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_IMPACT);
		effect.SetPosition(EffectManager::EffectType::PLAYER_IMPACT, this->collider->rigidbody.GetPosition());
		this->collider->data->isHitAttack = false;
		_player.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
	}

	/*当たり判定許可フラグが立っていなかったら早期リターン*/
	if (!this->collider->data->isDoHitCheck) return;

	/*当たり判定座標の更新*/
	VECTOR	direction	= VTransform(this->firstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));	//向きの設定
	VECTOR	position	= _player.GetRigidbody().GetPosition();													//プレイヤーの座標
			position	= VAdd(position, VScale(direction, this->POSITION_OFFSET));								//プレイヤーの座標に、オフセット値を足す
			position.y += this->Y_OFFSET;																		//Y座標オフセット値を足す
	this->collider->rigidbody.SetPosition(position);

	//再生時間が定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
	if (this->nowTotalPlayTime > this->END_HIT_CHECK_PLAY_TIME)
	{
		this->collider->data->isDoHitCheck = false;
	}

#ifdef _DEBUG
	DrawSphere3D(this->collider->rigidbody.GetPosition(), this->collider->radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
#endif // _DEBUG

}