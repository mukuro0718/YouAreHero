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
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossStabAction::BossStabAction()
	: isClose(false)
{
	this->attack = new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
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
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json	 = Singleton<JsonManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::STAB);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//初期化フラグを立てる
			this->isInitialize = true;
		}
	}

	/*ヒットストップの更新*/
	{
		//攻撃が当たっていたら
		if (this->attack->GetIsHitAttack())
		{
			//攻撃の種類
			const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::SLASH_1);
			//ヒットストップの設定
			this->hitStop->SetHitStop
			(
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE],
				static_cast<int>(HitStop::Type::STOP),
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE],
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE]
			);
			//攻撃ヒットフラグを下す
			this->attack->OffIsHitAttack();
		}
		//ヒットストップ中だったら早期リターン
		if (this->hitStop->IsHitStop()) return;
	}

	/*カウントの計測*/
	this->frameCount++;
	bool isRotation = false;	//回転するか
	bool isMove		= false;	//移動するか
	bool isStop		= false;	//停止するか
	{
			//フレームカウントが定数未満だったら許可フラグを立てる
		if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
		{
			this->isAllowAction = true;
		}
		//フレームカウントが定数以内なら移動フラグを立てる
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_FRAME"])
		{
			isMove = true;
		}
		//フレームカウントが定数以内なら移動フラグを立てる
		else if (this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_FRAME"])
		{
			isStop = true;
		}
		//フレームカウントが定数以内なら回転フラグを立てる
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_FRAME"])
		{
			isRotation = true;
		}
		//フレームカウントが定数以内なら移動目標を更新する
		if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["STAB_HOMING_FRAME"])
		{
			this->moveTarget = player.GetRigidbody().GetPosition();
		}
	}

	/*移動処理*/
	{
		//使用する値の準備
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//回転率の補完値
		VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
		VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);			//座標と移動目標間のベクトル
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);				//向き
		float  speed = 0.0f;																		//移動スピードの設定
		//回転処理
		{
			if (isRotation)
			{
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, LERP_VALUE);
				//回転率を設定
				_boss.SetRotation(nowRotation);
			}
		}
		//アクション許可フラグが立っていなければ早期リターン
		if (!this->isAllowAction)return;
		//移動速度の設定
		{
			//一度でも近づいていない＆＆移動フラグが立っていなかったら
			if (!this->isClose && !isMove && !isStop)
			{
				//座標と移動目標との距離を求める
				const float DISTANCE = VSize(positonToTargetVector);
				//距離が定数以上か
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
				}
				else
				{
					//近づいたフラグを立てる
					this->isClose = true;
				}
			}
		}
		//移動ベクトルの設定
		{
			//移動目標の設定
			_boss.SetNowMoveTarget(this->moveTarget);
			//新しい移動ベクトルの算出
			VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
			//移動ベクトルの設定
			_boss.SetVelocity(newVelocity);
		}

	}

	/*アニメーション処理*/
	{
		//アニメーションの設定
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::STAB));
		//アニメーション再生時間
		float animationPlayTime = _boss.GetAnimationPlayTime();
		//カウントが終了していなければアニメーション再生速度を遅くする
		if (isMove)
		{
			animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"];
		}
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*アニメーションの再生*/
		_boss.PlayAnimation();
	}

	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossStabAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*距離を求める*/
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//移動目標
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//距離

	this->parameter->desireValue = 0;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*状態がANGRYだったら欲求値を増加する*/
	else if (_boss.GetAngryState() >= static_cast<int>(Boss::BossState::ANGRY))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::STAB)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//コンボ数が残っていたら
			if (_boss.GetAttackComboCount() != 0)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}