#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "LoadingAsset.h"
#include "BossStateUI.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossStateUI::BossStateUI()
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->imageHandle.emplace_back(asset.GetImage(LoadingAsset::ImageType::TIRED_ICON));
	this->imageHandle.emplace_back(asset.GetImage(LoadingAsset::ImageType::NORMAL_ICON));
	this->imageHandle.emplace_back(asset.GetImage(LoadingAsset::ImageType::ANGRY_ICON));
}

/// <summary>
/// デストラクタ
/// </summary>
BossStateUI::~BossStateUI()
{

}

/// <summary>
/// 描画
/// </summary>
const void BossStateUI::Draw()const
{
	auto& map = Singleton<MapManager>::GetInstance();
	if (map.GetMapType() == MapManager::MapType::BOSS)
	{
		auto& json = Singleton<JsonManager>::GetInstance();
		auto& boss = Singleton<EnemyManager>::GetInstance();
		vector<int> drawRect = json.GetJson(JsonManager::FileType::UI)["BOSS_STATE_ICON_DRAW_RECT"];
		int bossState = boss.GetBossState();
		DrawExtendGraph(drawRect[0], drawRect[1], drawRect[2], drawRect[3], this->imageHandle[bossState], TRUE);
	}
}