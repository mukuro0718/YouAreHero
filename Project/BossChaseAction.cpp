#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossChaseAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossChaseAction::BossChaseAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossChaseAction::~BossChaseAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossChaseAction::Initialize()
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
void BossChaseAction::Update(Boss& _boss)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		/*シングルトンクラスのインスタンスの取得*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();

		/*使用する値の準備*/
		const VECTOR POSITION		= _boss.GetRigidbody().GetPosition();					//座標
		const VECTOR MOVE_TARGET	= player.GetRigidbody().GetPosition();					//移動目標
			  VECTOR nowRotation	= _boss.GetRigidbody().GetRotation();					//回転率
			  VECTOR direction		= VGet(0.0f, 0.0f, 0.0f);								//向き
		const float  SPEED			= json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];	//速さ

		/*移動ベクトルの設定*/
		_boss.SetNowMoveTarget(MOVE_TARGET);

		/*プレイヤーから自分の座標までのベクトルを出す*/
		VECTOR positionToTargetVector = VSub(POSITION, MOVE_TARGET);

		/*アークタンジェントを使って角度を求める*/
		nowRotation.y = static_cast<float>(atan2(static_cast<double>(positionToTargetVector.x), static_cast<double>(positionToTargetVector.z)));

		/*回転率を代入*/
		_boss.SetRotation(nowRotation);

		/*速さの設定*/
		_boss.SetSpeed(SPEED);

		/*回転率をもとに、移動する向きを出す*/
		direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));

		/*向きベクトルを正規化*/
		direction = VNorm(direction);

		/*移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）*/
		VECTOR aimVelocity  = VScale(direction, SPEED);								//算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル

		_boss.SetVelocity(newVelocity);

		/*移動目標との距離が定数以下だったら選択フラグを下す*/
		float distance = VSize(positionToTargetVector);
		if (distance <= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
		{
			OffIsSelect(0);
		}
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossChaseAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*追加する欲求値*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	///*もしボスとプレイヤーの間が定数以上離れていたら欲求値を倍増させる*/
	//else if (_distance >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	//{
	//	addDesireValue = this->parameter->MAX_PARAMETER;
	//}
	//else
	//{
	//	addDesireValue = 1;
	//}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}