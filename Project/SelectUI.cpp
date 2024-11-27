#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "SelectUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
SelectUI::SelectUI()
{
	/*画像ハンドルの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->background = asset.GetImage(LoadingAsset::ImageType::SELECT_BACK);
	this->logo = asset.GetImage(LoadingAsset::ImageType::SELECT_LOGO);
	this->provDecide = asset.GetImage(LoadingAsset::ImageType::SELECT_PROV_DECIDE);
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_1));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_2));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_3));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_4));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_1));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_2));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_3));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_4));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_1));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_2));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_3));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_4));
	this->iconFrame = asset.GetImage(LoadingAsset::ImageType::SELECT_ICON_FRAME);
}

/// <summary>
/// デストラクタ
/// </summary>
SelectUI::~SelectUI()
{
	this->icon.clear();
	this->image.clear();
	this->summary.clear();
}

/// <summary>
/// 初期化
/// </summary>
void SelectUI::Initialize()
{
	/*エネミーチェンジャーの初期化*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	enemyChanger.Initialize();
}


/// <summary>
/// 更新
/// </summary>
void SelectUI::Update()
{
	/*エネミーチェンジャーの更新*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	enemyChanger.Update();
	this->nowSelectEnemy = enemyChanger.GetEnemyType();

}

/// <summary>
/// 描画
/// </summary>
const void SelectUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*背景*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_BACKGROUND_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->background, TRUE);
	}

	/*イメージ画像*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_IMAGE_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->image[this->nowSelectEnemy], TRUE);
	}

	/*ロゴ*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_LOGO_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->logo, TRUE);
	}

	/*アイコン*/
	{
		vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"];
		for (int i = 0; i < position.size(); i++)
		{
			DrawExtendGraph(position[i][0], position[i][1], position[i][2], position[i][3], this->icon[i], TRUE);
		}
	}

	/*フレーム*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][this->nowSelectEnemy];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->iconFrame, TRUE);
	}

	/*サマリー*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_SUMMARY_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->summary[this->nowSelectEnemy], TRUE);
	}

	/*最終決定*/
	{
		auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
		if (enemyChanger.GetIsProvDecide())
		{
			vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_PROV_POSITION"];
			DrawExtendGraph(position[0], position[1], position[2], position[3], this->provDecide, TRUE);
		}
	}
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool SelectUI::IsEnd()const
{
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	return enemyChanger.GetIsFinalDecide();
}