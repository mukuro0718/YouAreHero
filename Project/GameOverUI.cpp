#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "GameOverUI.h"
#include "LoadingAsset.h"
#include "VECTORtoUseful.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverUI::GameOverUI()
	: FIRST_LOGO_RATE						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_LOGO_RATE"])
	, TARGET_LOGO_RATE_FOR_FADE_IN			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_LOGO_RATE_FOR_FADE_IN"	])
	, LERP_VALUE_FOR_REDUCTION				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_LERP_VALUE_FOR_REDUCTION"	])
	, WAIT_TIME_BEFORE_START_FADE_IN		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_WAIT_TIME_BEFORE_START_FADE_IN"])
	, WAIT_TIME_BEFORE_START_FADE_OUT		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_WAIT_TIME_BEFORE_START_FADE_OUT"])
	, INCREASED_ALPHA_FOR_LOGO_FADE_IN		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_INCREASED_ALPHA_FOR_LOGO_FADE_IN"])
	, REDUCTION_ALPHA_FOR_LOGO_FADE_OUT		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_REDUCTION_ALPHA_FOR_LOGO_FADE_OUT"])
	, POSITION_X							(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION_X"])
	, FIRST_POSITION_Y						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION_Y"])
	, POSITION_Y_FOR_FADE_OUT_MAGNIFICATION	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_POSITION_Y_FOR_FADE_OUT_MAGNIFICATION"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->questFailedLogo = asset.GetImage(LoadingAsset::ImageType::QUEST_FAILED);
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverUI::~GameOverUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameOverUI::Initialize()
{
	this->logoRate				= this->FIRST_LOGO_RATE;
	this->logoAlpha				= 0;
	this->waitTimer				= this->WAIT_TIME_BEFORE_START_FADE_IN;
	this->isProductFinished		= false;
	this->positionY				= this->FIRST_POSITION_Y;
	this->currentProductStage	= ProductStage::WAIT_FOR_FADE_IN;
}

/// <summary>
/// 更新
/// </summary>
void GameOverUI::Update()
{
	/*終了フラグが立っていたら早期リターン*/
	if (this->isProductFinished)return;

	switch (this->currentProductStage)
	{
	case ProductStage::WAIT_FOR_FADE_IN:
		//waitTimerを減らす
		this->waitTimer--;
		//waitTimerが0だったら段階を切り返る
		if (this->waitTimer == 0)
		{
			this->currentProductStage = ProductStage::LOGO_FADE_IN;
		}
		break;
	case ProductStage::LOGO_FADE_IN:
		//縮小処理
		this->logoRate = Gori::LerpFloat(this->logoRate, this->TARGET_LOGO_RATE_FOR_FADE_IN, this->LERP_VALUE_FOR_REDUCTION);
		//フェードイン
		this->logoAlpha += this->INCREASED_ALPHA_FOR_LOGO_FADE_IN;
		//フェードインが終わっていたら段階を切り返る
		if (this->logoAlpha >= this->MAX_ALPHA)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_OUT;
			this->waitTimer = this->WAIT_TIME_BEFORE_START_FADE_OUT;
		}
		break;
	case ProductStage::WAIT_FOR_FADE_OUT:
		//waitTimerを減らす
		this->waitTimer--;
		//waitTimerが0だったら段階を切り返る
		if (this->waitTimer == 0)
		{
			this->currentProductStage = ProductStage::LOGO_FADE_OUT;
		}
		break;
	case ProductStage::LOGO_FADE_OUT:
		//移動
		this->positionY += this->POSITION_Y_FOR_FADE_OUT_MAGNIFICATION;
		//フェードアウト
		this->logoAlpha -= this->REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;
		//フェードアウトが終わっていたら段階を切り返る
		if (this->logoAlpha <= 0)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_IN;
			this->isProductFinished = true;
		}
		break;
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameOverUI::Draw()const
{
	/*終了フラグが立っていたら早期リターン*/
	if (this->isProductFinished)return;

	/*ロゴの描画*/
	double rate = static_cast<double>(this->logoRate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->logoAlpha);
	DrawRotaGraph(this->POSITION_X, this->positionY, rate, this->LOGO_ANGLE, this->questFailedLogo, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);
}