#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "BossNameUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossNameUI::BossNameUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);
}

/// <summary>
/// デストラクタ
/// </summary>
BossNameUI::~BossNameUI()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossNameUI::Initialize()
{
}

/// <summary>
/// 更新
/// </summary>
void BossNameUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto&  enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

}

/// <summary>
/// 描画
/// </summary>
const void BossNameUI::Draw()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	vector<int>	 position = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"];
	vector<int> backGround = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_BACKGROUND"];
	DrawExtendGraph(backGround[0], backGround[1], backGround[2], backGround[3], this->imageHandle, TRUE);
	DrawStringToHandle(position[0], position[1], "ゴ ル グ グ ラ ス", this->TEXT_COLOR, this->fontHandle);
}

