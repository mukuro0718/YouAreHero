#include <DxLib.h>
#include "DeleteInstance.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "HPUI.h"
#include "ButtonUI.h"
#include "BossNameUI.h"
#include "SceneUI.h"
#include "GameUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameUI::GameUI()
	: hp(nullptr)
	, button(nullptr)
	, bossName(nullptr)
{
	/*インスタンスの生成*/
	this->hp = new HPUI();
	this->button = new ButtonUI();
	this->bossName = new BossNameUI();

	/*画像クラスインスタンスの作成*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->imageHandle		= asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
	this->fontHandle		= asset.GetFont(LoadingAsset::FontType::MINTYO_150_32);
	this->pauseFontHandle	= asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);

	auto& json = Singleton<JsonManager>	 ::GetInstance();
	this->maxAlpha					= json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"];
	this->addAlpha					= json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
	this->logoDrawTime				= json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"];
	vector<int> tableDrawRect		= json.GetJson(JsonManager::FileType::UI)["GAME_TABLE_DRAW_RECT"];
	this->tableDrawRect				= tableDrawRect;
	vector<int> destroyTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_1"];
	this->destroyTextPosition		= destroyTextPosition;
	vector<int> resultTextPosition	= json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_2"];
	this->resultTextPosition		= resultTextPosition;
	vector<int> pauseTableDrawRect = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TABLE_DRAW_RECT"];
	this->pauseTableDrawRect = pauseTableDrawRect;
	vector<int> pauseTextPosition = json.GetJson(JsonManager::FileType::UI)["GAME_PAUSE_TEXT_POSITION"];
	this->pauseTextPosition = pauseTextPosition;
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

	/*変数の初期化*/
	this->alpha = 0;
	this->isEnd = false;
	this->type = -1;
	this->frameCount = 0;
}


/// <summary>
/// 更新
/// </summary>
void GameUI::Update()
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
		//Bが押されていたら
		if (input.GetNowPadState() & InputManager::PAD_B)
		{
			this->type = static_cast<int>(Type::LOSE);
			this->isPause = false;
		}
		//Aが押されていたら
		else if (input.GetNowPadState() & InputManager::PAD_A)
		{
			this->isPause = false;
		}
	}

	this->hp->Update();
	this->button->Update();
	this->bossName->Update();

	/*タイプの設定*/
	SetType();

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;

	/*拡大が終了していなければ拡大して早期リターン*/
	if (this->alpha < this->maxAlpha)
	{
		this->alpha += this->addAlpha;
		return;
	}

	/*フレームカウントが定数以上だったら終了フラグを立てる*/
	this->frameCount++;
	if (this->frameCount >= this->logoDrawTime)
	{
		this->isEnd = true;
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameUI::Draw()const
{
	this->hp->Draw();
	this->button->Draw();
	this->bossName->Draw();

	/*中断フラグが立っていたらUIを表示する*/
	if (this->isPause)
	{
		DrawExtendGraph(this->pauseTableDrawRect[0], this->pauseTableDrawRect[1], this->pauseTableDrawRect[2], this->pauseTableDrawRect[3], this->imageHandle, TRUE);
		DrawStringToHandle(this->pauseTextPosition[0], this->pauseTextPosition[1], "ゲームを中断しますか\nB:はい A:いいえ", this->TEXT_COLOR, this->pauseFontHandle);

	}

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(this->tableDrawRect[0], this->tableDrawRect[1], this->tableDrawRect[2], this->tableDrawRect[3], this->imageHandle, TRUE);
	if (this->type == static_cast<int>(Type::LOSE))
	{
		DrawStringToHandle(this->destroyTextPosition[0], this->destroyTextPosition[1], "討\n伐", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(this->resultTextPosition[0], this->resultTextPosition[1], "失\n敗", this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawStringToHandle(this->destroyTextPosition[0], this->destroyTextPosition[1], "討\n伐", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(this->resultTextPosition[0], this->resultTextPosition[1], "完\n了", this->TEXT_COLOR, this->fontHandle);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->maxAlpha);
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool GameUI::IsEnd()const
{
	return this->isEnd;
}

/// <summary>
/// 種類の設定
/// </summary>
void GameUI::SetType()
{
	/*-1じゃなければ早期リターン*/
	if (this->type != -1)return;
	
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*プレイヤーのHPが０以下*/
	if (player.GetHP() <= 0 && !player.GetIsAlive())
	{
		this->type = static_cast<int>(Type::LOSE);
	}
	/*ボスのHPが０以下*/
	else if (enemy.GetHP() <= 0 && !enemy.GetIsAlive())
	{
		this->type = static_cast<int>(Type::WIN);
	}
}