#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "Character.h"
#include "Player.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "ScreenSetup.h"
#include "PlayerController.h"

/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI()
	: displayIndex(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->table = asset.GetImage(LoadingAsset::ImageType::POTION_TABLE);
	this->potion = asset.GetImage(LoadingAsset::ImageType::POTION);

	this->operationFont = asset.GetFont(LoadingAsset::FontType::MINTYO_80_64);
	this->itemFont = asset.GetFont(LoadingAsset::FontType::MINTYO_30_32);
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_WALK));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_RUN));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_AVOID));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_GUARD));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_DEVALUTION));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_ROUNDING_UP));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_DIVE_CUTTING));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_ROTARY_CUTTER));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_USE_ITEM));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_SWORD_DELIVERY));
	this->textList.emplace_back(asset.GetImage(LoadingAsset::ImageType::OPERATION_SWORD_DRAWING));

	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::A_BUTTON));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::B_BUTTON));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::X_BUTTON));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::Y_BUTTON));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::LEFT_STICK));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::RIGHT_TRIGGER));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::RIGHT_BUMPER));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::BUTTON_SET_1));
	this->buttonList.emplace_back(asset.GetImage(LoadingAsset::ImageType::BUTTON_SET_2));

	auto& json = Singleton<JsonManager>::GetInstance();
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_1));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_2));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::COMBO_3));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::STRONG_ATTACK));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::BLOCK));
	this->changeDisplayIndexType.emplace_back(static_cast<int>(PlayerController::PlayerState::DRAW_SWORD_1));

	for (int i = 0; i < this->changeDisplayIndexType.size(); i++)
	{
		this->displayTextMap.emplace(this->changeDisplayIndexType[i], json.GetJson(JsonManager::FileType::UI)["DISPLAY_TEXT_INDEX"][i]);
		this->displayButtonMap.emplace(this->changeDisplayIndexType[i], json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_INDEX"][i]);
	}
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
	this->displayIndex = static_cast<int>(PlayerController::PlayerState::DRAW_SWORD_1);
}

/// <summary>
/// 更新
/// </summary>
void ButtonUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	int playerNowState = player.GetNowState();
	for (int i = 0; i < this->changeDisplayIndexType.size(); i++)
	{
		if (this->changeDisplayIndexType[i] == playerNowState)
		{
			this->displayIndex = playerNowState;
			return;
		}
	}
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
	DrawItem();

	/*ボタンの描画*/
	DrawButton();

	/*アイコンごとの文字の描画*/
	DrawTextImage();
}


/// <summary>
/// アイコンの描画
/// </summary>
void ButtonUI::DrawItem()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の準備*/
	vector<int> potionPositon = json.GetJson(JsonManager::FileType::UI)["ITEM_POSITION"];
	vector<int> tablePosition = json.GetJson(JsonManager::FileType::UI)["ITEM_TABLE_POSITION"];
	vector<int> buttonPosition = json.GetJson(JsonManager::FileType::UI)["ITEM_BUTTON_POSITION"];
	vector<int> numPosition = json.GetJson(JsonManager::FileType::UI)["ITEM_NUM_POSITION"];
	vector<int> numTablePosition = json.GetJson(JsonManager::FileType::UI)["ITEM_NUM_TABLE_POSITION"];
	int radius = json.GetJson(JsonManager::FileType::UI)["RADIUS"];
	int thickness = json.GetJson(JsonManager::FileType::UI)["THICKNESS"];
	vector<int> tableColorBase = json.GetJson(JsonManager::FileType::UI)["TABLE_COLOR"];
	vector<int> numColorBase = json.GetJson(JsonManager::FileType::UI)["NUM_COLOR"];
	int tableColor = GetColor(tableColorBase[0], tableColorBase[1], tableColorBase[2]);
	int numColor = GetColor(numColorBase[0], numColorBase[1], numColorBase[2]);
	int buttonType = json.GetJson(JsonManager::FileType::UI)["ITEM_BUTTON_TYPE"];

	/*jsonデータの代入*/
	const int itemNum = player.GetHealOrbNum();

	/*アイテムの描画*/
	DrawExtendGraph(potionPositon[0], potionPositon[1], potionPositon[2], potionPositon[3], this->table, TRUE);
	
	/*テーブルの描画*/
	DrawExtendGraph(tablePosition[0], tablePosition[1], tablePosition[2], tablePosition[3], this->potion, TRUE);
	
	/*ボタンの描画*/
	DrawExtendGraph(buttonPosition[0], buttonPosition[1], buttonPosition[2], buttonPosition[3], this->buttonList[buttonType], TRUE);

	/*アイテムの数*/
	DrawCircle(numTablePosition[0], numTablePosition[1], radius, tableColor,1);
	DrawFormatStringToHandle(numPosition[0], numPosition[1], numColor, this->itemFont, "×%d", itemNum);
}

void ButtonUI::DrawButton()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_POSITION"];
	int width = 0;
	int height = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_HEIGHT"];
	vector<int> drawList = this->displayButtonMap[this->displayIndex];
	for (int i = 0; i < drawList.size(); i++)
	{
		int buttonType = drawList[i];
		width = json.GetJson(JsonManager::FileType::UI)["DISPLAY_BUTTON_WIDTH"][buttonType];
		DrawExtendGraph(position[0], position[1], position[0] + width, position[1] + height, this->buttonList[buttonType], TRUE);
		position[1] += height;
	}
}

/// <summary>
/// 文字の描画
/// </summary>
void ButtonUI::DrawTextImage()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();
	auto& screen = Singleton<ScreenSetup>::GetInstance();

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["DISPLAY_TEXT_POSITION"];
	int width = json.GetJson(JsonManager::FileType::UI)["DISPLAY_TEXT_WIDTH"];
	int height = json.GetJson(JsonManager::FileType::UI)["DISPLAY_TEXT_HEIGHT"];
	vector<int> drawList = this->displayTextMap[this->displayIndex];
	for (int i = 0; i < drawList.size(); i++)
	{
		int textType = drawList[i];
		DrawExtendGraph(position[0], position[1], position[0] + width, position[1] + height, this->textList[textType], TRUE);
		position[1] += height;
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