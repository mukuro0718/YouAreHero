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
#include "BossStabAttack.h"
#include "BossStabAction.h"
#include "PlayerManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossStabAction::BossStabAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::STAB))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"])
	, SLOW_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"])
	, MIN_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	, MAX_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["MIDDLE_DISTANCE"])
	, SLOW_LIMIT_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_LIMIT_TIME"])
	, CHECK_STATE	(static_cast<int>(Boss::BossState::ANGRY))
{
	this->attack			= new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
	auto& json				= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::STAB);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
BossStabAction::~BossStabAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossStabAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->isAllowAction			 = false;
	this->frameCount			 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossStabAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	/*初期化されていなかったら初期化*/
	if (!this->isInitialize)
	{
		//攻撃タイプの設定
		_boss.SetAttackType(Boss::AttackType::STAB);
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		//初期化フラグを立てる
		this->isInitialize = true;
		//アニメーションの設定
		_boss.SetNowAnimation(this->nextAnimation);
	}

	/*ヒットストップの更新*/
	//攻撃が当たっていたら
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//ヒットストップの設定
		this->hitStop->SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//攻撃ヒットフラグを下す
		this->attack->OffIsHitAttack();
	}
	//ヒットストップ中だったら早期リターン
	if (this->hitStop->IsHitStop()) return;

	/*フレームカウントが回転定数以上だったら許可フラグを立てる*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}
	else
	{
		this->moveTarget = player.GetRigidbody().GetPosition();
	}

	/*現在のアニメーション再生時間によってフラグを決める*/
	bool isRotation = false;	//回転するか
	bool isMove		= false;	//移動するか
	//再生時間が定数以内なら移動フラグを立てる
	if (this->nowTotalAnimPlayTime < this->MOVE_PLAY_TIME)
	{
		isMove = true;
	}
	//再生時間が定数以内なら移動フラグを立てる
	else if (this->nowTotalAnimPlayTime >= this->STOP_PLAY_TIME)
	{
		isMove = false;
	}
	//再生時間が定数以内なら回転フラグを立てる
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
	}

	/*移動処理*/
	//使用する値の準備
	const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//座標
		  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
		  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//座標と移動目標間のベクトル
		  VECTOR direction	 = VGet(0.0f, 0.0f, 0.0f);				//向き
		  float  speed		 = 0.0f;								//移動スピードの設定
	//回転処理
	if (isRotation)
	{
		//回転率を補完する
		nowRotation = GetLerpRotation(_boss, toTarget, nowRotation, this->ROTATE_LERP_VALUE);
		//回転率を設定
		_boss.SetRotation(nowRotation);
	}
	//アクション許可フラグが立っていなければ早期リターン
	if (!this->isAllowAction)return;
	//移動速度の設定
	//一度でも近づいていない＆＆移動フラグが立っていなかったら
	if (!this->isClose && isMove)
	{
		//座標と移動目標との距離を求める
		const float DISTANCE = VSize(toTarget);
		//距離が定数以上か
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		else
		{
			//近づいたフラグを立てる
			this->isClose = true;
		}
	}
	//移動ベクトルの設定
	//移動目標の設定
	_boss.SetNowMoveTarget(this->moveTarget);
	//新しい移動ベクトルの算出
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
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
		this->isInitialize = false;
		this->isClose = false;
		this->nowTotalAnimPlayTime = 0.0f;
		//その他変数の初期化とインターバルのセット
		OffIsSelect(this->MAX_INTERVAL);
		//コンボ数を減らす
		_boss.DecAttackComboCount();
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossStabAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*状態がANGRYだったら欲求値を増加する*/
	if (_boss.GetAngryState() == this->CHECK_STATE)
	{
		/*距離を求める*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//移動目標
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//目標から現在の移動目標へのベクトル
		const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//距離
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (this->MIN_ACTION_DISTANCE <= DISTANCE && DISTANCE <= this->MAX_ACTION_DISTANCE)
		{
			//コンボ数が残っていたら
			if (_boss.GetAttackComboCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}