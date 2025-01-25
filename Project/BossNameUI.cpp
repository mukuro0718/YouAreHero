#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "BossNameUI.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossNameUI::BossNameUI()
	: fontHandle(-1)
	, imageHandle(-1)
{
	/*画像ハンドルの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->fontHandle  = asset.GetFont(LoadingAsset::FontType::BOSS_NAME);
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
	/*変数の初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	int			 enemyType	 = enemyChanger.GetEnemyType();
	vector<int>	 position	 = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"][enemyType];
	vector<int>  background  = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_BACKGROUND"][enemyType];
	string		 name		 = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_TYPE"][enemyType];
	this->namePosition		 = position;
	this->backgroundDrawRect = background;
	this->bossName			 = name;
}

/// <summary>
/// 更新
/// </summary>
void BossNameUI::Update()
{
}

/// <summary>
/// 描画
/// </summary>
const void BossNameUI::Draw()const
{
	//DrawExtendGraph(this->backgroundDrawRect[0], this->backgroundDrawRect[1], this->backgroundDrawRect[2], this->backgroundDrawRect[3], this->imageHandle, TRUE);
	DrawStringToHandle(this->namePosition[0], this->namePosition[1], this->bossName.c_str(), this->TEXT_COLOR, this->fontHandle);
}

