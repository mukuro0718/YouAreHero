#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
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
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		/*シングルトンクラスのインスタンスの取得*/
		auto& json = Singleton<JsonManager>::GetInstance();

		/*使用する値の準備*/
		const float  SPEED		= 0.0f;
		const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();
			  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);			 //向き

		/*スピードを０にする*/
		_boss.SetSpeed(SPEED);

		/*回転率をもとに、移動する向きを出す*/
		direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));

		/*向きベクトルを正規化*/
		direction = VNorm(direction);

		/*移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）*/
		VECTOR aimVelocity = VScale(direction, SPEED);								//算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル

		/*移動ベクトルの設定*/
		_boss.SetVelocity(newVelocity);

		//フレーム計測
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
		//フレーム計測が終了していたら
		if (isEndCount)
		{
			OffIsSelect(0);
		}
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	/*追加する欲求値*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	///*怒りのゲージに比例して、欲求ゲージを決める*/
	//else
	//{
	//	this->parameter->desireValue = this->parameter->MAX_PARAMETER - _angryValue;
	//}
	this->parameter->AddDesireValue(addDesireValue);
}