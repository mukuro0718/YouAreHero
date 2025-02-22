#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlash2Attack.h"
#include "BossSlash2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlash2Action::BossSlash2Action()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::SLASH_2))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_INTERVAL"])
	, SLOW_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_SLOW_PLAY_TIME"])
	, ACTION_DISTANCE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][this->ATTACK_TYPE])
	, SLOW_LIMIT_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_SLOW_LIMIT_TIME"])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::NORMAL))
{
	this->attack			= new BossSlash2Attack(static_cast<int>(BossAttack::AttackType::SLASH_2));
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
}

/// <summary>
/// デストラクタ
/// </summary>
BossSlash2Action::~BossSlash2Action()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlash2Action::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->attack->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void BossSlash2Action::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }


	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::SLASH_2);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//移動目標の設定
			this->moveTarget = player.GetRigidbody().GetPosition();
			//初期化フラグを立てる
			this->isInitialize = true;
			//アニメーションの設定
			_boss.SetNowAnimation(this->nextAnimation);
			//アニメーション再生時間
			_boss.SetAnimationPlayTime(this->animationPlayTime);
		}
	}

	/*ヒットストップ*/
	{
		//ヒットストップの更新
		if (this->attack->GetIsHitAttack())
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
			//ヒットストップの設定
			_boss.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
			//攻撃ヒットフラグを下す
			this->attack->OffIsHitAttack();
		}
	}

	/*フレームカウントが回転定数以上だったら許可フラグを立てる*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}

	/*移動処理*/		
	VECTOR position		= _boss.GetRigidbody().GetPosition();	//座標
	VECTOR nowRotation	= _boss.GetRigidbody().GetRotation();	//回転率
	VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);			//向き
	float  speed		= 0.0f;									//移動の速さ
	bool   isRotation	= false;								//回転するか
	bool   isMove		= false;								//移動するか
	//再生時間が定数範囲以内なら移動フラグを立てる
	if (this->nowTotalAnimPlayTime < this->MOVE_PLAY_TIME)
	{
		isMove = true;
	}
	//再生時間が定数以内なら回転フラグを立てる
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
		this->moveTarget = player.GetRigidbody().GetPosition();
	}
	//回転フラグが立っていたら
	if (isRotation)
	{
		//座標と移動目標間のベクトル
		VECTOR positonToTargetVector = VSub(position, this->moveTarget);
		//回転率を補完する
		nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->ROTATE_LERP_VALUE);
		//回転率を代入
		_boss.SetRotation(nowRotation);
	}
	//一度でも近づいていない＆＆移動フラグが立っていたら
	if (!this->isClose && isMove)
	{
		//座標と移動目標間のベクトル
		VECTOR positonToTargetVector = VSub(position, this->moveTarget);
		//座標と移動目標との距離を求める
		const float DISTANCE = VSize(positonToTargetVector);
		//距離が定数以上だったら移動速度を代入する
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		//距離が未満だったらフラグを立てる
		else
		{
			this->isClose = true;
		}
	}
	//移動目標の設定
	_boss.SetNowMoveTarget(this->moveTarget);
	//回転率をもとに移動ベクトルを出す
	direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
	//移動ベクトルを正規化
	direction = VNorm(direction);
	//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
	VECTOR aimVelocity = VScale(direction, speed);					 //算出された移動ベクトル
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //前の移動ベクトル
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
	//移動ベクトルの設定
	_boss.SetVelocity(newVelocity);

	/*アニメーション処理*/
	float playTime = 0.0f;
	if (this->nowTotalAnimPlayTime <= this->SLOW_LIMIT_TIME)
	{
		playTime = this->SLOW_PLAY_TIME;
	}
	else
	{
		playTime = this->animationPlayTime;
	}
	this->nowTotalAnimPlayTime += playTime;
	_boss.SetAnimationPlayTime(playTime);
	//アニメーションの再生
	_boss.PlayAnimation();

	/*攻撃判定の更新*/
	this->attack->Update(this->nowTotalAnimPlayTime);

	/*終了判定*/
	//アニメーションが終了していたら
	if (_boss.GetIsChangeAnimation())
	{
		//各フラグを下す
		this->isInitialize			= false;
		this->isClose				= false;
		this->nowTotalAnimPlayTime	= 0.0f;
		//その他変数の初期化とインターバルのセット
		OffIsSelect(this->MAX_INTERVAL);
		//コンボ数を減らす
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossSlash2Action::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*インターバルが残っていたら早期リターン*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*状態がNORMALだったら欲求値を増加する*/
	if (_boss.GetBossState() == this->CHECK_STATE)
	{
		/*距離を求める*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR position		= _boss.GetRigidbody().GetPosition();	//座標
		VECTOR moveTarget	= player.GetRigidbody().GetPosition();	//移動目標
		VECTOR toTarget		= VSub(position, moveTarget);			//目標から現在の移動目標へのベクトル
		float  distance		= VSize(toTarget);						//距離

		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (distance <= this->ACTION_DISTANCE)
		{
			//コンボ数が残っていなかったらリターン
			if (_boss.GetAttackCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}