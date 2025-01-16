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
	: background	(-1)
	, iconTable		(-1)
	, frame			(-1)
	, provDecide	(-1)
	, nowSelectEnemy(0)
	, isEnd			(false)
{
	/*アセットの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->headerFont		 = asset.GetFont(LoadingAsset::FontType::SELECT_HEADER);
	this->questFont			 = asset.GetFont(LoadingAsset::FontType::SELECT_QUEST);
	this->actionFont		 = asset.GetFont(LoadingAsset::FontType::SELECT_ACTION);
	this->background		 = asset.GetImage(LoadingAsset::ImageType::SELECT_BACK);
	this->provDecide		 = asset.GetImage(LoadingAsset::ImageType::SELECT_PROV_DECIDE);
	this->frame				 = asset.GetImage(LoadingAsset::ImageType::SELECT_FRAME);
	this->iconTable			 = asset.GetImage(LoadingAsset::ImageType::SELECT_ICON_TABLE);
	this->enemyImageTable	 = asset.GetImage(LoadingAsset::ImageType::SELECT_IMAGE_TABLE);
	this->drawRectBackGround = asset.GetImage(LoadingAsset::ImageType::SELECT_DRAW_RECT);
	this->aButton = asset.GetImage(LoadingAsset::ImageType::A_BUTTON);
	this->bButton = asset.GetImage(LoadingAsset::ImageType::B_BUTTON);
	this->lStick = asset.GetImage(LoadingAsset::ImageType::LEFT_STICK_INPUT);
	this->enemyImage.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_1));
	this->enemyImage.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_2));
	this->enemyImage.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_3));
	this->enemyImage.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_4));

}

/// <summary>
/// デストラクタ
/// </summary>
SelectUI::~SelectUI()
{
	this->enemyImage.clear();
}

/// <summary>
/// 初期化
/// </summary>
void SelectUI::Initialize()
{
	/*エネミーチェンジャーの初期化*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	enemyChanger.Initialize();
	this->nowSelectEnemy = 0;
	this->isEnd = false;
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
	vector<int> fontColorIndex = json.GetJson(JsonManager::FileType::UI)["SELECT_FONT_COLOR"];
	int fontColor = GetColor(fontColorIndex[0], fontColorIndex[1], fontColorIndex[2]);
	/*背景*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_BACKGROUND_POSITION"];
		vector<int> headerPosition = json.GetJson(JsonManager::FileType::UI)["SELECT_MENU_HEADER_POSITION"];
		vector<int> headerLinePosition = json.GetJson(JsonManager::FileType::UI)["SELECT_MENU_HEADER_LINE_POSITION"];
		vector<int> colorIndex = json.GetJson(JsonManager::FileType::UI)["SELECT_BACKGROUND_COLOR"];
		int color = GetColor(colorIndex[0], colorIndex[1], colorIndex[2]);
		DrawBox(position[0], position[1], position[2], position[3], color, TRUE);
		DrawStringToHandle(headerPosition[0], headerPosition[1], "選択画面", fontColor, this->headerFont);
		DrawLine(headerLinePosition[0], headerLinePosition[1], headerLinePosition[2], headerLinePosition[3], fontColor);

	}


	/*描画範囲*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_DRAW_RECT_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->drawRectBackGround, TRUE);//全体の枠
	}

	/*イメージ画像*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_IMAGE_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->enemyImageTable, TRUE);
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->enemyImage[this->nowSelectEnemy], TRUE);
	}

	/*ロゴ*/
	{
		//vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_LOGO_POSITION"];
		
	}

	/*アイコン*/
	{
		int iconNum = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_NUM"];
		vector<int> positionOffset = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_TEXT_OFFSET"];
		vector<int> position1 = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][0];
		vector<int> position2 = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][1];
		vector<int> position3 = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][2];
		vector<int> position4 = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][3];
		vector<int> headerPosition = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_HEADER_POSITION"];
		vector<int> headerLinePosition = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_HEADER_LINE_POSITION"];

		DrawStringToHandle(headerPosition[0], headerPosition[1], "クエスト一覧", fontColor, this->headerFont);
		DrawLine(headerLinePosition[0], headerLinePosition[1], headerLinePosition[2], headerLinePosition[3], fontColor);
		DrawExtendGraph(position1[0], position1[1], position1[2], position1[3], this->iconTable, TRUE);
		DrawStringToHandle(position1[0] + positionOffset[0], position1[1] + positionOffset[1], "討伐：GORG GRASS", fontColor, this->questFont);
		DrawExtendGraph(position2[0], position2[1], position2[2], position2[3], this->iconTable, TRUE);
		DrawStringToHandle(position2[0] + positionOffset[0], position2[1] + positionOffset[1], "討伐：LUXURIO", fontColor, this->questFont);
		DrawExtendGraph(position3[0], position3[1], position3[2], position3[3], this->iconTable, TRUE);
		DrawStringToHandle(position3[0] + positionOffset[0], position3[1] + positionOffset[1], "討伐：MORNACT", fontColor, this->questFont);
		DrawExtendGraph(position4[0], position4[1], position4[2], position4[3], this->iconTable, TRUE);
		DrawStringToHandle(position4[0] + positionOffset[0], position4[1] + positionOffset[1], "チュートリアル", fontColor, this->questFont);
	}

	/*フレーム*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][this->nowSelectEnemy];
		int frameOffset = json.GetJson(JsonManager::FileType::UI)["SELECT_FRAME_POSITION_OFFSET"];
		DrawExtendGraph(position[0] - frameOffset, position[1] - frameOffset, position[2] + frameOffset, position[3] + frameOffset, this->frame, TRUE);
	}

	/*アクション*/
	{
		vector<int> decidePosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_DECIDE_TEXT_POSITION"];
		vector<int> decideButtonPosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_DECIDE_BUTTON_POSITION"];
		vector<int> backPosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_BACK_TEXT_POSITION"];
		vector<int> backButtonPosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_BACK_BUTTON_POSITION"];
		vector<int> selectPosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_SELECT_TEXT_POSITION"];
		vector<int> selectButtonPosition	= json.GetJson(JsonManager::FileType::UI)["SELECT_SELECT_BUTTON_POSITION"];
		DrawStringToHandle(decidePosition[0], decidePosition[1], "決定", fontColor, this->actionFont);
		DrawExtendGraph(decideButtonPosition[0], decideButtonPosition[1], decideButtonPosition[2], decideButtonPosition[3], this->bButton, TRUE);
		DrawStringToHandle(backPosition[0], backPosition[1], "戻る", fontColor, this->actionFont);
		DrawExtendGraph(backButtonPosition[0], backButtonPosition[1], backButtonPosition[2], backButtonPosition[3], this->aButton, TRUE);
		DrawStringToHandle(selectPosition[0], selectPosition[1], "選択", fontColor, this->actionFont);
		DrawExtendGraph(selectButtonPosition[0], selectButtonPosition[1], selectButtonPosition[2], selectButtonPosition[3], this->lStick, TRUE);

		//vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_SUMMARY_POSITION"];
		//DrawExtendGraph(position[0], position[1], position[2], position[3], this->summary[this->nowSelectEnemy], TRUE);
	}

	//DrawBox(100 + 1, 150 + 1, 1820 - 1, 980 - 1, GetColor(255, 0, 0), FALSE);//全体の枠
	//DrawBox(760 + 1, 50 + 1, 1160 - 1, 100 - 1, GetColor(255, 0, 0), FALSE);//画面の説明
	//DrawBox(960 + 1, 50 + 1, 960 - 1, 100 - 1, GetColor(255, 0, 0), FALSE);//画面の説明
	//DrawBox(250 + 1, 300 + 1, 885 - 1, 700 - 1, GetColor(255, 0, 0), FALSE);//アイコンの位置y:75+25
	//DrawBox(400 + 1, 200 + 1, 735 - 1, 250 - 1, GetColor(255, 0, 0), FALSE);//アイコンの説明
	//DrawBox(910 + 1, 250 + 1, 1820 - 1, 880 - 1, GetColor(255, 0, 0), FALSE);//写真の位置
	

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