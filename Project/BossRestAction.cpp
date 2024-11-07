#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRestAction::BossRestAction()
{

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
	this->maxFrameCount			 = 0;
	this->prevAngryState		 = 1;
	this->nowAnimationType		 = -1;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*カラースケールの変更*/
	{
		//初期化フラグが立っていなかったらカラースケールを更新する
		if (this->isChangeColorScale)
		{
			//ボスのモデルハンドル
			const int MODEL_HANDLE = _boss.GetModelHandle();
			//初期化フラグが立っていたら現在の色を取得する
			if (!this->isInitializeColorScale)
			{
				//カラースケールの更新(ここでは赤色になるようにする)
				for (int i = 0; i < this->nowColorScale.size(); i++)
				{
					this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
				}
				this->isInitializeColorScale = true;
			}
			//カラースケールの更新(ここでは赤色になるようにする)
			for (int i = 0; i < this->nowColorScale.size(); i++)
			{
				const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["REST_TARGET_COLOR_SCALE"]);
				const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
				this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
				this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
			}
		}
	}

	/*怒り状態を合わせる*/
	this->prevAngryState = _boss.GetAngryState();

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
			this->nowAnimationType = static_cast<int>(Boss::AnimationType::DOWN);
		}
		else
		{
			this->nowAnimationType = static_cast<int>(Boss::AnimationType::DOWN_UP);
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
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	if (_boss.GetHP() <= 0)return;

	/*怒り状態*/
	int nowAngryState = _boss.GetAngryState();
	/*AngryStateがTIREDの時に*/
	if (nowAngryState == static_cast<int>(Boss::AngryStateType::TIRED))
	{
		//保存している状態と異なっていたら
		if (nowAngryState != this->prevAngryState)
		{
			this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			this->isPriority			 = true;
			this->isChangeColorScale	 = true;
			this->isInitializeColorScale = false;
		}
	}

	/*違う状態なら現在の状態を保存する*/
	else
	{
		this->prevAngryState = _boss.GetAngryState();
	}

	/*ボスのAngryTypeをもとに最大フレームも決めておく*/
	this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["REST_ACTION_MAX_FRAME"];
}