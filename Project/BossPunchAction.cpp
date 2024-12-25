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
#include "BossPunchAttack.h"
#include "BossPunchAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossPunchAction::BossPunchAction()
{
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::PUNCH);
	this->attack			= new BossPunchAttack(ATTACK_TYPE);
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->rotateFixFrame	= json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"];
	this->rotateFrame		= json.GetJson(JsonManager::FileType::ENEMY)["PUNCH_ROTATE_FRAME"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::PUNCH);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["PUNCH_INTERVAL"];
	this->checkState		= static_cast<int>(Boss::BossState::NORMAL);
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->normalDesireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];

}

/// <summary>
/// デストラクタ
/// </summary>
BossPunchAction::~BossPunchAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossPunchAction::Initialize()
{
	this->isSelect = false;
	this->isInitialize = false;
	this->frameCount = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossPunchAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::PUNCH);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//初期化フラグを立てる
			this->isInitialize = true;
		}
	}

	/*ヒットストップの更新*/
	if (this->attack->GetIsHitAttack())
	{
		//ヒットストップの設定
		this->hitStop->SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
		//攻撃ヒットフラグを下す
		this->attack->OffIsHitAttack();
		//ヒットストップ中だったら早期リターン
		if (this->hitStop->IsHitStop()) return;
	}

	/*カウントの計測*/
	this->frameCount++;

	/*移動処理*/
	{
		//使用する値の準備
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();		//座標
			  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();		//回転率
			  VECTOR positonToTargetVector	= VSub(POSITION, this->moveTarget);	//プレイヤーから自分の座標までのベクトル
			  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);				//向き
			  float  speed					= 0.0f;										//移動スピード
			  bool   isRotation				= false;									//回転するか
		{
			//フレームカウントが回転定数未満だったら許可フラグを立てる
			if (this->frameCount > this->rotateFixFrame)
			{
				this->isAllowAction = true;
			}
			//フレームカウントが定数以内なら回転フラグを立てる
			if (this->frameCount < this->rotateFrame)
			{
				isRotation = true;
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//回転処理
		{
			if (isRotation)
			{
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->rotateLerpValue);
				//回転率を設定
				_boss.SetRotation(nowRotation);
			}
		}
		//移動ベクトルの設定
		{
			//移動目標の設定
			_boss.SetNowMoveTarget(this->moveTarget);
			//回転率をもとに移動ベクトルを出す
			direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
			//移動ベクトルを正規化
			direction = VNorm(direction);
			//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
			VECTOR aimVelocity = VScale(direction, speed);							//算出された移動ベクトル
			VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				//前の移動ベクトル
			VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
			//移動ベクトルの設定
			_boss.SetVelocity(newVelocity);
		}
	}

	/*アニメーション処理*/
	{
		//アニメーションの設定
		_boss.SetNowAnimation(this->nextAnimation);
		//再生時間の設定
		_boss.SetAnimationPlayTime(this->animationPlayTime);
		//アニメーションの再生
		_boss.PlayAnimation();
	}



	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		OffIsSelect(this->maxInterval);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossPunchAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0))
	{
		return;
	}

	/*状態がTIRED,NORMAL,ANGRYだったら欲求値を増加する*/
	if (_boss.GetAngryState() >= this->checkState)
	{
		/*距離を求める*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
		const float  DISTANCE = VSize(POSITION_TO_TARGET);			//距離

		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= this->actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//コンボが０だったら早期リターン
			if (_boss.GetAttackComboCount() == 0)return;
			//前の攻撃がSTABだったら
			if (type == Boss::AttackType::STAB)
			{
				this->parameter->desireValue = this->maxDesireValue;
			}
			//それ以外だったら
			else
			{
				this->parameter->desireValue = this->normalDesireValue;
			}
		}
	}
}