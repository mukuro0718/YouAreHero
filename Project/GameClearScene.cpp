#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "GameClearScene.h"
#include "LoadingAsset.h"
#include "VECTORtoUseful.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameClearScene::GameClearScene()
	: FIRST_LOGO_ANGLE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_FIRST_LOGO_ANGLE"])
	, TARGET_LOGO_ANGLE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_ANGLE"])
	, FIRST_LOGO_RATE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_FIRST_LOGO_RATE"])
	, TARGET_LOGO_RATE_FOR_FADE_IN					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_RATE_WITH_FADE_IN"])
	, TARGET_LOGO_RATE_FOR_FADE_OUT					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_RATE_WITH_FADE_OUT"])
	, TARGET_EFFECT_LOGO_RATE						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_EFFECT_LOGO_RATE"])
	, LERP_VALUE_FOR_ANGLE							(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_ANGLE"])
	, LERP_VALUE_FOR_REDUCTION						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_REDUCTION"])
	, LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION"])
	, LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFIGATION"])
	, WAIT_TIME_BEFORE_START_FADE_IN				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WAIT_TIME_BEFORE_START_FADE_IN"])
	, WAIT_TIME_BEFORE_START_FADE_OUT				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WAIT_TIME_BEFORE_START_FADE_OUT"])
	, INCREASED_ALPHA_FOR_LOGO_FADE_IN				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_INCREASED_ALPHA_FOR_LOGO_FADE_IN"])
	, REDUCTION_ALPHA_FOR_LOGO_FADE_OUT				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_REDUCTION_ALPHA_FOR_LOGO_FADE_OUT"])
	, REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->questClearLogo = asset.GetImage(LoadingAsset::ImageType::QUEST_CLEAR);
}

/// <summary>
/// デストラクタ
/// </summary>
GameClearScene::~GameClearScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameClearScene::Initialize()
{
	this->logoAngle			= this->FIRST_LOGO_ANGLE;
	this->logoRate			= this->FIRST_LOGO_RATE;
	this->logoAlpha			= 0;
	this->effectLogoRate	= 0.0f;
	this->effectLogoAlpha	= 0;
	this->waitTimer			= this->WAIT_TIME_BEFORE_START_FADE_IN;
	this->isProductFinished = false;
}

/// <summary>
/// 更新
/// </summary>
void GameClearScene::Update()
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
		//回転処理
		this->logoAngle = Gori::LerpFloat(this->logoAngle, this->TARGET_LOGO_ANGLE, this->LERP_VALUE_FOR_ANGLE);
		//縮小処理
		this->logoRate = Gori::LerpFloat(this->logoRate, this->TARGET_LOGO_RATE_FOR_FADE_IN, this->LERP_VALUE_FOR_REDUCTION);
		//フェードイン
		this->logoAlpha++;
		//フェードインが終わっていたら段階を切り返る
		if (this->logoAlpha == this->MAX_ALPHA)
		{
			this->currentProductStage = ProductStage::EFFECT_FADE_OUT;
			this->effectLogoAlpha	  = this->logoAlpha;
			this->effectLogoRate	  = this->logoRate;
		}
		break;
	case ProductStage::EFFECT_FADE_OUT:
		//拡大処理
		this->effectLogoRate = Gori::LerpFloat(this->effectLogoRate, this->TARGET_EFFECT_LOGO_RATE, this->LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION);
		//フェードアウト
		this->effectLogoAlpha++;
		//フェードアウトが終わっていたら段階を切り返る
		if (this->effectLogoAlpha == 0)
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
		//拡大処理
		this->logoRate = Gori::LerpFloat(this->effectLogoRate, this->TARGET_LOGO_RATE_FOR_FADE_OUT, this->LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION);
		//フェードアウト
		this->logoAlpha++;
		//フェードアウトが終わっていたら段階を切り返る
		if (this->logoAlpha == 0)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_IN;
			this->isProductFinished	  = true;
		}
		break;
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameClearScene::Draw()const
{
	/*終了フラグが立っていたら早期リターン*/
	if (this->isProductFinished)return;

	/*ロゴの描画*/
	double rate = static_cast<double>(this->logoRate);
	double angle = static_cast<double>(this->logoAngle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->logoAlpha);
	DrawRotaGraph(this->DRAW_POSITION_X, DRAW_POSITION_Y, rate, angle, this->questClearLogo, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);

	/*エフェクト用ロゴの描画*/
	if (this->currentProductStage == ProductStage::EFFECT_FADE_OUT)
	{
		double rate = static_cast<double>(this->effectLogoRate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->effectLogoAlpha);
		DrawRotaGraph(this->DRAW_POSITION_X, DRAW_POSITION_Y, rate, angle, this->questClearLogo, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);
	}
}