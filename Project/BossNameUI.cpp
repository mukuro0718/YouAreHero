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

	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_150_64));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_70_64));
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
	this->positionType = 0;
	this->drawTextIndex = 0;
	this->addTextIndexInterval = 0;
	this->entryInterval = 0;
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

	/*座標の種類の変更*/
	ChangePositionType();

	/*テキストインデックスの追加*/
	AddTextIndex();
}

/// <summary>
/// 描画
/// </summary>
const void BossNameUI::Draw()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	const vector<string> TEXT	  = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME"];
	const vector<int>	 OFFSET   = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_OFFSET"][this->positionType];
		  vector<int>	 position = json.GetJson(JsonManager::FileType::UI)["BOSS_NAME_POSITION"][this->positionType];
	for (int i = 0; i < this->drawTextIndex; i++)
	{
		DrawStringToHandle(position[0], position[1], TEXT[i].c_str(), this->TEXT_COLOR, this->fontHandle[this->positionType]);
		position[0] += OFFSET[i];
	}
}

/// <summary>
/// テキストインデックスの追加
/// </summary>
void BossNameUI::AddTextIndex()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	if (player.GetIsAlive())
	{
		if (this->drawTextIndex < json.GetJson(JsonManager::FileType::UI)["BOSS_NAME"].size())
		{
			this->addTextIndexInterval++;
			if (this->addTextIndexInterval >= json.GetJson(JsonManager::FileType::UI)["TEXT_INDEX_INTERVAL"])
			{
				this->addTextIndexInterval = 0;
				this->drawTextIndex++;
			}
		}
	}
}

/// <summary>
/// 座標の種類の変更
/// </summary>
void BossNameUI::ChangePositionType()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	if (player.GetIsAlive())
	{
		if (this->entryInterval < json.GetJson(JsonManager::FileType::CAMERA)["ENTRY_CAMERA_INTERVAL"])
		{
			this->positionType = static_cast<int>(PositionType::ENTRY);
			this->entryInterval++;
		}
		else
		{
			this->positionType = static_cast<int>(PositionType::NORMAL);
		}
	}
}