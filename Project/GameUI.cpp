#include <DxLib.h>
#include "DeleteInstance.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "SceneBase.h"
#include "Image.h"
#include "HPUI.h"
#include "BossStateUI.h"
#include "ButtonUI.h"
#include "BossNameUI.h"
#include "GameClearUI.h"
#include "GameOverUI.h"
#include "SceneUI.h"
#include "GameUI.h"
#include "TutorialUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameUI::GameUI()
	: hp				(nullptr)
	, button			(nullptr)
	, bossName			(nullptr)
	, LOCK_ON_UI_OFFSET	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_LOCK_ON_POSITION_OFFSET"])
	, LOCK_ON_UI_SIZE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_LOCK_ON_SIZE"])
{
	/*インスタンスの生成*/
	this->hp		= new HPUI();
	this->button	= new ButtonUI();
	this->bossName	= new BossNameUI();
	this->clearUI	= new GameClearUI();
	this->overUI	= new GameOverUI();
	this->tutorialUI= new TutorialUI();
	this->stateUI	= new BossStateUI();

	/*画像クラスインスタンスの作成*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->backGround		= asset.GetImage(LoadingAsset::ImageType::SELECT_IMAGE_TABLE);
	this->pauseFontHandle	= asset.GetFont(LoadingAsset::FontType::SELECT_HEADER);
	this->pauseActionHandle = asset.GetFont(LoadingAsset::FontType::SELECT_QUEST);
	this->lockOnImage		= asset.GetImage(LoadingAsset::ImageType::ICON_LOCK_ON);
	this->decideButton		= asset.GetImage(LoadingAsset::ImageType::B_BUTTON);
	this->backButton		= asset.GetImage(LoadingAsset::ImageType::A_BUTTON);

	auto& json = Singleton<JsonManager>	 ::GetInstance();
	this->maxAlpha		= json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"];
	this->addAlpha		= json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
	this->logoDrawTime	= json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"];


	vector<int> pauseTableDrawRect		= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TABLE_DRAW_RECT"];
	vector<int> pauseHeaderTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_HEADER_TEXT_POSITION"];
	vector<int> pauseActionTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_ACTION_TEXT_POSITION"];
	vector<int> decideButtonDrawRect	= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_DECIDE_DRAW_RECT"];
	vector<int> backButtonDrawRect		= json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_BACK_DRAW_RECT"];
	this->pauseTableDrawRect		= pauseTableDrawRect;
	this->pauseHeaderTextPosition	= pauseHeaderTextPosition;
	this->pauseActionTextPosition	= pauseActionTextPosition;
	this->decideButtonDrawRect		= decideButtonDrawRect;
	this->backButtonDrawRect		= backButtonDrawRect;
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameUI::~GameUI()
{
	DeleteMemberInstance(this->hp);
	DeleteMemberInstance(this->button);
	DeleteMemberInstance(this->bossName);
}

/// <summary>
/// 初期化
/// </summary>
void GameUI::Initialize()
{
	/*クラスの初期化*/
	this->hp->Initialize();
	this->button->Initialize();
	this->bossName->Initialize();
	this->clearUI->Initialize();
	this->overUI->Initialize();
	this->tutorialUI->Initialize();

	/*変数の初期化*/
	this->alpha						= 0;
	this->isEnd						= false;
	this->type						= ResultType::NONE;
	this->frameCount				= 0;
	this->alphaForTransition		= this->MAX_ALPHA;
	this->isTransition				= true;
	this->isEndFadeInForTransition  = false;

}


/// <summary>
/// 更新
/// </summary>
void GameUI::Update()
{
	/*画面遷移用のアルファ値が残っていたら*/
	if (this->isTransition)
	{
		//遷移用のフェードインが終わって居なかったら
		if (!this->isEndFadeInForTransition)
		{
			this->alphaForTransition -= this->ALPHA_REDUCTION;
			if (this->alphaForTransition <= 0)
			{
				this->isEndFadeInForTransition = true;
				this->isTransition = false;
			}
		}
		else
		{
			this->alphaForTransition += this->ALPHA_INCREASE;
		}
	}

	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	if (enemyChanger.GetEnemyType() == static_cast<int>(EnemyChanger::EnemyType::TUTORIAL))
	{
		this->tutorialUI->Update();
	}

	this->hp->Update();
	this->button->Update();
	this->bossName->Update();
	
	if (this->type == ResultType::NONE)
	{
		/*中断UIをするかどうかの判定*/
		auto& input = Singleton<InputManager>::GetInstance();
		//中断フラグが立っていなければ
		if (!this->isPause)
		{
			//PADのSTARTが立っていたら
			if (input.GetNowPadState() & InputManager::PAD_START)
			{
				this->isPause = true;
			}
		}
		//立っていたら
		else
		{
			//Aが押されていたら
			if (input.GetNowPadState() & InputManager::PAD_A)
			{
				this->type = ResultType::LOSE;
				this->isPause = false;
			}
			//Bが押されていたら
			else if (input.GetNowPadState() & InputManager::PAD_B)
			{
				this->isPause = false;
			}
		}


		/*タイプの設定*/
		SetType();
	}
	else
	{
		if (this->type == ResultType::LOSE)
		{
			this->overUI->Update();
			this->isEnd = this->overUI->GetIsProductFinished();
		}
		else
		{
			this->clearUI->Update();
			this->isEnd = this->clearUI->GetIsProductFinished();
		}
		if (this->isEnd)
		{
			this->isTransition = true;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameUI::Draw()const
{
	this->tutorialUI->Draw();
	this->hp->Draw();
	this->stateUI->Draw();
	this->button->Draw();
	this->bossName->Draw();

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == ResultType::NONE)
	{
		/*中断フラグが立っていたらUIを表示する*/
		if (this->isPause)
		{
			DrawExtendGraph(this->pauseTableDrawRect[0], this->pauseTableDrawRect[1], this->pauseTableDrawRect[2], this->pauseTableDrawRect[3], this->backGround, TRUE);
			DrawExtendGraph(this->decideButtonDrawRect[0], this->decideButtonDrawRect[1], this->decideButtonDrawRect[2], this->decideButtonDrawRect[3], this->backButton, TRUE);
			DrawExtendGraph(this->backButtonDrawRect[0], this->backButtonDrawRect[1], this->backButtonDrawRect[2], this->backButtonDrawRect[3], this->decideButton, TRUE);
			DrawStringToHandle(this->pauseHeaderTextPosition[0], this->pauseHeaderTextPosition[1], "ゲームを中断しますか", this->TEXT_COLOR, this->pauseFontHandle);
			DrawStringToHandle(this->pauseActionTextPosition[0], this->pauseActionTextPosition[1], ":はい       :いいえ", this->TEXT_COLOR, this->pauseActionHandle);
		}

		auto& player = Singleton<PlayerManager>::GetInstance();
		if (player.GetIsLockOn())
		{
			auto& enemy = Singleton<EnemyManager>::GetInstance();
			VECTOR position = enemy.GetPositionForLockon();
			//position.y += this->LOCK_ON_UI_OFFSET;
			DrawBillboard3D(position, 0.5f, 0.5f, this->LOCK_ON_UI_SIZE, 0.0f, this->lockOnImage, TRUE);
		}
	}
	else
	{
		if (this->type == ResultType::LOSE)
		{
			this->overUI->Draw();
		}
		else
		{
			this->clearUI->Draw();
		}
	}

	/*シーン遷移の描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alphaForTransition);
	DrawBox(0, 0, this->MAX_X, this->MAX_Y, 0, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool GameUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	if (this->isEndFadeInForTransition && this->alphaForTransition >= this->MAX_ALPHA)
	{
		return true;
	}
	return false;
}

/// <summary>
/// 種類の設定
/// </summary>
void GameUI::SetType()
{
	/*-1じゃなければ早期リターン*/
	if (this->type != ResultType::NONE)return;
	
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*プレイヤーのHPが０以下*/
	if (player.GetHP() <= 0 && !player.GetIsAlive())
	{
		this->type = ResultType::LOSE;
	}
	/*ボスのHPが０以下*/
	else if (enemy.GetHP() <= 0 && !enemy.GetIsAlive())
	{
		this->type = ResultType::WIN;
	}
}