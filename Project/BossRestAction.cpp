#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRestAction::BossRestAction()
	: maxFrameCount (0)
	, nextAnimation2(0)
	, nowAnimationType(0)
	, isChangeColorScale(false)
	, prevState(0)
	, isInitializeColorScale(false)
	, isDownUp(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::DOWN);
	this->nextAnimation2	= static_cast<int>(Boss::AnimationType::DOWN_UP);
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->maxFrameCount		= json.GetJson(JsonManager::FileType::ENEMY)["REST_ACTION_MAX_FRAME"];
	this->checkedState		= static_cast<int>(Boss::BossState::TIRED);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
BossRestAction::~BossRestAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossRestAction::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isInitializeColorScale = false;
	this->isChangeColorScale	 = false;
	this->isDownUp				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->prevState = 1;
	this->nowAnimationType		 = -1;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*怒り状態を合わせる*/
	if (this->frameCount == 0)
	{
		this->prevState = _boss.GetBossState();
	}

	/*移動処理*/
	{
		//使用する値の準備
		const float  SPEED		= 0.0f;								 //スピード
		const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();//回転率
			  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);		 //向き
		//スピードの設定
		_boss.SetSpeed(SPEED);
		//回転率をもとに、移動する向きを出す
		direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		//向きベクトルを正規化
		direction = VNorm(direction);
		//移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）
		VECTOR aimVelocity = VScale(direction, SPEED);					//算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				//前の移動ベクトル
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
		//移動ベクトルの設定
		_boss.SetVelocity(newVelocity);
	}
	
	/*フレーム計測*/
	bool isEndCount = FrameCount(this->maxFrameCount);

	/*アニメーションの処理*/
	{
		float animationPlayTime = _boss.GetAnimationPlayTime();
		bool isPlayAnimation = true;
		//カウントが終了していなければ
		if (!isEndCount)
		{
			if (_boss.GetIsChangeAnimation() && this->nowAnimationType != -1)
			{
				isPlayAnimation = false;
			}
			this->nowAnimationType = this->nextAnimation;
		}
		else
		{
			this->nowAnimationType = this->nextAnimation2;
			this->isDownUp = true;
		}
		//アニメーション再生フラグが立っていたら
		if (isPlayAnimation)
		{
			//アニメーションの設定
			_boss.SetNowAnimation(this->nowAnimationType);
			//再生時間の設定
			_boss.SetAnimationPlayTime(animationPlayTime);
			//アニメーションの再生
			_boss.PlayAnimation();
		}
	}

	/*フレーム計測が終了していたら*/
	if (isEndCount && _boss.GetIsChangeAnimation())
	{
		OffIsSelect(0);
		this->isChangeColorScale = false;
		this->nowAnimationType = -1;
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	if (_boss.GetHP() <= 0)return;

	/*AngryStateがTIREDの時に*/
	int nowState = _boss.GetBossState();
	if (nowState == this->checkedState)
	{
		this->parameter->desireValue = this->maxDesireValue;
		this->isPriority = true;
		this->isChangeColorScale = true;
		this->isInitializeColorScale = false;
	}

	/*違う状態なら現在の状態を保存する*/
	else
	{
		this->prevState = nowState;
	}

}