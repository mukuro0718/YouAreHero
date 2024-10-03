#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "LoadingAsset.h"
#include "InputManager.h"
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

	this->iconFont		= asset.GetFont(LoadingAsset::FontType::HONOKA_50_64);
	this->operationFont = asset.GetFont(LoadingAsset::FontType::HONOKA_30_64);

	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::B_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::X_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::Y_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::LT_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_B_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_X_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_Y_BUTTON));
	this->button.emplace_back(asset.GetImage(LoadingAsset::ImageType::PRESS_LT_BUTTON));
	this->buttonFont = asset.GetFont(LoadingAsset::FontType::BAT_32_0);
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

	/*ボタンの描画*/
	DrawButton();

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

void ButtonUI::DrawButton()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_POSITION"];
	int pad = input.GetPadState();
	vector<bool> isTrigger;
	isTrigger.emplace_back(pad & PAD_INPUT_4);
	isTrigger.emplace_back(pad & PAD_INPUT_1);
	isTrigger.emplace_back(pad & PAD_INPUT_2);
	isTrigger.emplace_back(pad & PAD_INPUT_7);

	int drawGraph = -1;
	for (int i = 0; i < position.size(); i++)
	{
		if (isTrigger[i])
		{
			drawGraph = this->button[i + this->PRESS_OFFSET];
		}
		else
		{
			drawGraph = this->button[i];
		}
		DrawExtendGraph(position[i][0], position[i][1], position[i][2], position[i][3], drawGraph, TRUE);
	}
}

/// <summary>
/// 背景の描画
/// </summary>
void ButtonUI::DrawFont()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["BUTTON_TEXT_POSITION"];
	vector<string> text = json.GetJson(JsonManager::FileType::UI)["BUTTON_TEXT"];
	int pad = input.GetPadState();
	vector<bool> isTrigger;
	isTrigger.emplace_back(pad & PAD_INPUT_4);
	isTrigger.emplace_back(pad & PAD_INPUT_1);
	isTrigger.emplace_back(pad & PAD_INPUT_2);
	isTrigger.emplace_back(pad & PAD_INPUT_7);

	int textColor = 0;
	for (int i = 0; i < text.size(); i++)
	{
		if (isTrigger[i])
		{
			textColor = this->PRESS_TEXT_COLOR;
		}
		else
		{
			textColor = this->TEXT_COLOR;
		}
		DrawStringToHandle(position[i][0], position[i][1], text[i].c_str(), textColor, this->buttonFont);
	}
}
/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}