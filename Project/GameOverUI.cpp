#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameOverUI.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverUI::GameOverUI()
	: isPrevPressButton	(false)
	, isContinue		(false)
	, isEnd				(false)
	, type				(-1)
	, imageHandle		(-1)
	, fontHandle		(-1)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);

	/*fontHandleの取得*/
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_64);
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
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	this->alpha				= 0;
	this->isContinue		= false;
	this->isPrevPressButton = false;
	this->type				= 0;
}


/// <summary>
/// 更新
/// </summary>
void GameOverUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	bool isPressButton = IsPressButton();

	SetType();

	/*拡大が終了していなければ拡大して早期リターン*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*ボタンが押されていたらリザルト終了*/
	if (isPressButton)
	{
		if (this->type == static_cast<int>(ImageType::CONTINUE))
		{
			this->isContinue = true;
		}
		this->isEnd = true;
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameOverUI::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*背景の描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TABLE_POSITION"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);

	/*文字の描画*/
	vector<int> position1 = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TEXT_POSITION"][0];
	vector<int> position2 = json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TEXT_POSITION"][1];
	if (this->type == static_cast<int>(ImageType::CONTINUE))
	{
		DrawStringToHandle(position1[0], position1[1], "再戦", this->PRESS_TEXT_COLOR, this->fontHandle, TRUE);
		DrawStringToHandle(position2[0], position2[1], "終了", this->TEXT_COLOR, this->fontHandle, TRUE);
	}
	else
	{
		DrawStringToHandle(position1[0], position1[1], "再戦", this->TEXT_COLOR, this->fontHandle, TRUE);
		DrawStringToHandle(position2[0], position2[1], "終了", this->PRESS_TEXT_COLOR, this->fontHandle, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool GameOverUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	return this->isEnd;
}

/// <summary>
/// ゲームオーバー時、アイコンの種類を設定
/// </summary>
void GameOverUI::SetType()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	const int Y_BUF = input.GetLStickState().YBuf;

	/*上*/
	if (Y_BUF > 0)
	{
		this->type++;
		if (this->type >= static_cast<int>(ImageType::END))
		{
			this->type = static_cast<int>(ImageType::END);
		}
	}
	/*下*/
	else if (Y_BUF < 0)
	{
		this->type--;
		if (this->type <= static_cast<int>(ImageType::CONTINUE))
		{
			this->type = static_cast<int>(ImageType::CONTINUE);
		}
	}
}

/// <summary>
/// ボタンの入力
/// </summary>
bool GameOverUI::IsPressButton()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad入力*/
	int pad = input.GetNowPadState();
	bool isPressButton = false;

	/*windowが移動していない/拡大もしていなければ*/
	isPressButton = (pad & PAD_INPUT_4);
	//前にボタン入力がない&今ボタン入力がある
	if (!this->isPrevPressButton && isPressButton)
	{
		this->isPrevPressButton = true;
	}
	//前にボタン入力がある
	else if (this->isPrevPressButton)
	{
		//今ボタン入力がない
		if (!isPressButton)
		{
			this->isPrevPressButton = false;
		}
		isPressButton = false;
	}

	return isPressButton;
}


const bool GameOverUI::IsContinue()const
{
	return this->isContinue;
}