#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "BossJumpAttackAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossJumpAttackAction::BossJumpAttackAction()
	: isClose(false)
{
	this->attack = new BossJumpAttack(static_cast<int>(BossAttack::AttackType::JUMP_ATTACK));
}

/// <summary>
/// デストラクタ
/// </summary>
BossJumpAttackAction::~BossJumpAttackAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossJumpAttackAction::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossJumpAttackAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::JUMP_ATTACK);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//移動目標の設定
			const VECTOR TARGET = player.GetRigidbody().GetPosition();
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition();										//座標
			VECTOR positonToTargetVector = VSub(POSITION, TARGET);											//座標と移動目標間のベクトル
			VECTOR targetOffset = VNorm(VCross(VNorm(positonToTargetVector), Gori::UP_VEC));
			this->moveTarget = VAdd(TARGET, VScale(targetOffset, json.GetJson(JsonManager::FileType::ENEMY)["SLASH_TARGET_OFFSET"]));
			//初期化フラグを立てる
			this->isInitialize = true;
		}
	}

	/*ヒットストップ*/
	{
		//攻撃が当たっていたら
		if (this->attack->GetIsHitAttack())
		{
			//攻撃の種類
			const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::JUMP_ATTACK);
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
		//ヒットストップをしていたら早期リターン
		if (this->hitStop->IsHitStop()) return;
	}


	/*カウントの計測*/
	this->frameCount++;

	/*移動処理*/
	{
		//使用する変数
		VECTOR position				 = _boss.GetRigidbody().GetPosition();										//座標
		VECTOR lerpValue			 = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//回転率の補完値
		VECTOR nowRotation			 = _boss.GetRigidbody().GetRotation();										//回転率
		VECTOR positonToTargetVector = VSub(position, this->moveTarget);											//座標と移動目標間のベクトル
		VECTOR direction			 = VGet(0.0f, 0.0f, 0.0f);													//向き
		float  speed				 = 0.0f;																		//移動スピードの設定
		bool   isRotation			 = false;																	//回転するか
		bool   isMove				 = false;																	//移動するか
		{
			//フレームカウントが定数未満だったら許可フラグを立てる
			if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
			{
				this->isAllowAction = true;
			}
			//フレームカウントが定数以内なら移動フラグを立てる
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_MOVE_FRAME"])
			{
				isMove = true;
			}
			//フレームカウントが定数以内なら回転フラグを立てる
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_ROTATE_FRAME"])
			{
				isRotation = true;
			}
			//フレームカウントが定数以内なら移動目標を更新する
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_HOMING_FRAME"])
			{
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//回転処理
		{
			if (isRotation)
			{
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, lerpValue);
				//回転率を設定
				_boss.SetRotation(nowRotation);
			}
		}
		//アクション許可フラグが立っていなければ早期リターン
		if (!this->isAllowAction)return;
		//移動速度の設定
		{
			//一度も一定距離に近づいていない && 移動フラグが立っていたら
			if (!isMove)
			{
				//座標と移動目標との距離を求める
				const float DISTANCE = VSize(positonToTargetVector);
				//距離が定数以上なら速度を設定する
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_MOVE_SPEED"];
				}
				//一定未満ならフラグを立てる
				else
				{
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
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::JUMP_ATTACK));
		//再生時間の設定
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		//アニメーションの再生
		_boss.PlayAnimation();
	}

	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossJumpAttackAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*距離を求める*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
	const float  DISTANCE = VSize(POSITION_TO_TARGET);			//距離

	this->parameter->desireValue = 0;
	
	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*状態がNORMALかANGRYだったら欲求値を増加する*/
	else if (_boss.GetAngryState() >= static_cast<int>(Boss::BossState::NORMAL))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::JUMP_ATTACK)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//もしコンボが０だったら早期リターン
			if (_boss.GetAttackComboCount() == 0)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}

}
