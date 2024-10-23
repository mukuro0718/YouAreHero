#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossDeathAction.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossDeathAction::BossDeathAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossDeathAction::~BossDeathAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossDeathAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}


/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeathAction::Update(Boss& _boss)
{
	/*選択されていたら欲求値を０にする*/
	this->parameter->desireValue = 0;

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::DYING));

	/*アニメーションの再生*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	_boss.PlayAnimation();

	/*もしアニメーションが終了していたら*/
	if (_boss.GetIsChangeAnimation())
	{
		_boss.OffIsAlive();
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeathAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*HPが０以下だったら欲求値を最大にし、優先フラグを立てる*/
	if (_boss.GetHP() <= 0)
	{
		this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
		this->isPriority = true;
	}
	/*それ以外なら欲求値を０にして優先フラグを下す*/
	else
	{
		this->parameter->desireValue = 0;
		this->isPriority = false;
	}
}