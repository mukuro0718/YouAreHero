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

/// <summary>
/// コンストラクタ
/// </summary>
BossStabAction::BossStabAction()
	: isClose(false)
{
	this->attack			= new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
	auto& json				= Singleton<JsonManager>::GetInstance();
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::STAB);
	this->hitStopTime		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE];
	this->hitStopType		= static_cast<int>(HitStop::Type::STOP);
	this->hitStopDelay		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE];
	this->slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE];
	this->moveFrame			= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_FRAME"];
	this->stopFrame			= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_FRAME"];
	this->rotateFrame		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_FRAME"];
	this->rotateFixFrame	= json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"];
	this->homingFrame		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_HOMING_FRAME"];
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->stopDistance		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"];
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::STAB);
	this->slowPlayTime		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"];
	this->actionDistance	= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][ATTACK_TYPE];
	this->normalDisireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
	this->checkState		= static_cast<int>(Boss::BossState::ANGRY);
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
			//アニメーションの設定
			_boss.SetNowAnimation(this->nextAnimation);
		}
	}

	/*ヒットストップの更新*/
	{
		//攻撃が当たっていたら
		if (this->attack->GetIsHitAttack())
		{
			//攻撃の種類
			//ヒットストップの設定
			this->hitStop->SetHitStop(this->hitStopTime, this->hitStopType, this->hitStopDelay, this->slowFactor);
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
		if (this->frameCount > this->rotateFixFrame)
		{
			this->isAllowAction = true;
		}
		//フレームカウントが定数以内なら移動フラグを立てる
		if (this->frameCount < this->moveFrame)
		{
			isMove = true;
		}
		//フレームカウントが定数以内なら移動フラグを立てる
		else if (this->frameCount >= this->stopFrame)
		{
			isStop = true;
		}
		//フレームカウントが定数以内なら回転フラグを立てる
		if (this->frameCount < this->rotateFrame)
		{
			isRotation = true;
		}
		//フレームカウントが定数以内なら移動目標を更新する
		if (this->frameCount < this->homingFrame)
		{
			this->moveTarget = player.GetRigidbody().GetPosition();
		}
	}

	/*移動処理*/
	{
		//使用する値の準備
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//座標
			  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
			  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//座標と移動目標間のベクトル
			  VECTOR direction	 = VGet(0.0f, 0.0f, 0.0f);				//向き
			  float  speed		 = 0.0f;								//移動スピードの設定
		//回転処理
		{
			if (isRotation)
			{
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, toTarget, nowRotation, this->rotateLerpValue);
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
				const float DISTANCE = VSize(toTarget);
				//距離が定数以上か
				if (DISTANCE >= this->stopDistance)
				{
					speed = this->speed;
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
		//アニメーション再生時間
		float animationPlayTime = this->animationPlayTime;
		//カウントが終了していなければアニメーション再生速度を遅くする
		if (isMove)
		{
			animationPlayTime = this->slowPlayTime;
		}
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*アニメーションの再生*/
		_boss.PlayAnimation();
	}

	/*攻撃判定の更新*/
	this->attack->Update(0.0f);

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(this->maxInterval);
		_boss.DecAttackComboCount();
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossStabAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*距離を求める*/
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//移動目標
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//目標から現在の移動目標へのベクトル
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//距離

	this->parameter->desireValue = 0;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*状態がANGRYだったら欲求値を増加する*/
	else if (_boss.GetAngryState() >= this->checkState)
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE >= actionDistance)
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//コンボ数が残っていたら
			if (_boss.GetAttackComboCount() != 0)
			{
				this->parameter->desireValue = this->normalDisireValue;
			}
		}
	}
}