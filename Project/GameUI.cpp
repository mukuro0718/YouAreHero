#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameUI.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameUI::GameUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	const int GAME_CLEAR = asset.GetImage(LoadingAsset::ImageType::GAME_CLEAR);
	const int GAME_OVER = asset.GetImage(LoadingAsset::ImageType::GAME_OVER);
	this->image.emplace_back(new Image(GAME_CLEAR));
	this->image.emplace_back(new Image(GAME_OVER));
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameUI::~GameUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameUI::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = Image::MAX_ALPHA;
		this->image[i]->isAddAlpha = false;
		this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_FIRST_DRAW_RECT"]);
	}
	this->isPrevPressAButton = false;
	this->isEndExtend = false;
	this->isEnd = false;
	this->type = -1;
	this->frameCount = 0;
}


/// <summary>
/// 更新
/// </summary>
void GameUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	/*タイプの設定*/
	SetType();

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;

	/*拡大が終了していなければ拡大して早期リターン*/
	if (!this->isEndExtend)
	{
		this->isEndExtend = this->image[this->type]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_TARGET_DRAW_RECT"], json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_ADD_VALUE"]);
		return;
	}

	/*フレームカウントが定数未満だったらフレームカウントを増加して早期リターン*/
	if (this->frameCount < json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"])
	{
		this->frameCount++;
		return;
	}

	/*ここでは縮小させる*/
	this->isEnd = this->image[this->type]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_FIRST_DRAW_RECT"], json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DECREASE_VALUE"]);
	if (!isEnd)return;
}

/// <summary>
/// 描画
/// </summary>
const void GameUI::Draw()const
{
	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;

	this->image[this->type]->Draw();
	
	if (this->type == static_cast<int>(Type::LOSE))
	{

	}
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