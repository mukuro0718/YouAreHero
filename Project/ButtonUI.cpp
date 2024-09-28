#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->healIcon = asset.GetImage(LoadingAsset::ImageType::HEAL_ICON);
	this->healOrb  = asset.GetImage(LoadingAsset::ImageType::HP_ORB);
	this->emptyOrb = asset.GetImage(LoadingAsset::ImageType::EMPTY_ORB);

	this->iconFont		= asset.GetFont(LoadingAsset::FontType::ICON_UI);
	this->operationFont = asset.GetFont(LoadingAsset::FontType::OPERATION_UI);
}

/// <summary>
/// デストラクタ
/// </summary>
ButtonUI::~ButtonUI()
{

}

/// <summary>
/// 初期化
/// </summary>
void ButtonUI::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*初期化*/
}

/// <summary>
/// 更新
/// </summary>
void ButtonUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*回復アイコンのリキャストタイムを表示できるようにしたい*/
	
}

/// <summary>
/// 描画
/// </summary>
void ButtonUI::Draw()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*アイコンの描画*/
	DrawIcon();

	/*アイコンごとの文字の描画*/
	DrawFont();
}


/// <summary>
/// アイコンの描画
/// </summary>
void ButtonUI::DrawIcon()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の準備*/
	Vec2d healIconPosition;
	Vec2d orbPosition;
	int orbXOffset = 0;
	int drawGraph = -1;

	/*jsonデータの代入*/
	const int NOW_ORB_NUM = player.GetHealOrbNum();
	const int MAX_ORB_NUM = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];
	const int ORB_WIDTH   = json.GetJson(JsonManager::FileType::UI)["ORB_WIDTH"];
	orbPosition.Set(		json.GetJson(JsonManager::FileType::UI)["ORB_POSITION"]);
	healIconPosition.Set(	json.GetJson(JsonManager::FileType::UI)["HEAL_ICON_POSITION"]);


	/*アイコンテーブルの描画*/
	DrawGraph(healIconPosition.x, healIconPosition.y, this->healIcon, TRUE);

	/*オーブの描画*/
	for (int i = 0; i < MAX_ORB_NUM; i++)
	{
		if (i - NOW_ORB_NUM < 0)
		{
			drawGraph = this->healOrb;
		}
		else
		{
			drawGraph = this->emptyOrb;
		}
		DrawGraph(orbPosition.x + orbXOffset, orbPosition.y, drawGraph, TRUE);
		orbXOffset += ORB_WIDTH;
	}
}

/// <summary>
/// 背景の描画
/// </summary>
void ButtonUI::DrawFont()
{
	///*シングルトンクラスのインスタンスの取得*/
	//auto& json = Singleton<JsonManager>::GetInstance();

	///*変数の準備*/
	//Vec2d mainAttackPosition,specialAttackPosition,avoidPosition,blockPosition,dashPosition;
	//int   textColor;

	///*jsonデータの代入*/
	//mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	//specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	//avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	//blockPosition			.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	//dashPosition			.Set(json.GetJson(JsonManager::FileType::UI)["DASH_TEXT_POSITION"]);
	//textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);


	///*背景の描画*/
	//DrawStringToHandle(mainAttackPosition.x	  , mainAttackPosition.y	, "攻撃"	 , textColor, this->iconFont);
	//DrawStringToHandle(specialAttackPosition.x, specialAttackPosition.y	, "スキル"  , textColor, this->iconFont);
	//DrawStringToHandle(avoidPosition.x		  , avoidPosition.y			, "回避"	 , textColor, this->iconFont);
	//DrawStringToHandle(blockPosition.x		  , blockPosition.y			, "ガード"	 , textColor, this->iconFont);
	//DrawStringToHandle(dashPosition.x		  , dashPosition.y			, "ダッシュ", textColor, this->iconFont);
}
/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}