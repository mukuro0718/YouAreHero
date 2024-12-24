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
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_150_32);
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
	auto& json = Singleton<JsonManager>	 ::GetInstance();
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
	this->hp->Update();
	this->button->Update();
	this->bossName->Update();

	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	/*タイプの設定*/
	SetType();

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;

	/*拡大が終了していなければ拡大して早期リターン*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*フレームカウントが定数以上だったら終了フラグを立てる*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::UI)["GAME_LOGO_DRAW_TIME"])
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

	/*まだゲームが終了していなければ早期リターン*/
	if (this->type == -1)return;
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_TABLE_DRAW_RECT"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);
	vector<int> position1 = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_1"];
	vector<int> position2 = json.GetJson(JsonManager::FileType::UI)["GAME_TEXT_POSITION_2"];
	if (this->type == static_cast<int>(Type::LOSE))
	{
		DrawStringToHandle(position1[0], position1[1], "討\n伐", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(position2[0], position2[1], "失\n敗", this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawStringToHandle(position1[0], position1[1], "討\n伐", this->TEXT_COLOR, this->fontHandle);
		DrawStringToHandle(position2[0], position2[1], "完\n了", this->TEXT_COLOR, this->fontHandle);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);
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
	if (player.GetHP() < 0 && !player.GetIsAlive())
	{
		this->type = static_cast<int>(Type::LOSE);
	}
	/*ボスのHPが０以下*/
	else if (enemy.GetHP() < 0 && !enemy.GetIsAlive())
	{
		this->type = static_cast<int>(Type::WIN);
	}
}