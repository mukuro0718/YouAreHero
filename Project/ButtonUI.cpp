#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ButtonUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI()
{
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
	//サイズ
	this->specialAttackSize = 0;
}

/// <summary>
/// 更新
/// </summary>
void ButtonUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*特別な攻撃に対するフィルターの処理を行う（クールタイムゲージ）*/
	if (this->specialAttackSize <= json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"])
	{
		this->specialAttackSize++;
	}
	
}

/// <summary>
/// 描画
/// </summary>
void ButtonUI::Draw()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	/*背景の描画*/
	DrawBackground();

	/*アイコンの描画*/
	DrawIcon();

	/*アイコンごとの文字の描画*/
	DrawIconText();

	/*ボタン文字*/
	DrawButtonText();

	/*操作説明UIの描画*/
	DrawOperation();
}

void ButtonUI::DrawOperation()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Box   dashIcon1, dashIcon2, dashIcon3;
	Vec2d dashText;
	Vec2d dashButton1, dashButton2, dashButton3;
	int iconColor, buttonColor, textColor,fontSize;

	/*jsonの代入*/
	dashIcon1	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_1_POSITION"]);
	dashIcon3	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_3_POSITION"]);
	dashText	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_ICON_TEXT_POSITION"]);
	dashButton1	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_1_TEXT_POSITION"]);
	dashButton2	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_2_TEXT_POSITION"]);
	dashButton3	.Set(json.GetJson(JsonManager::FileType::UI)["DASH_BUTTON_3_TEXT_POSITION"]);
	iconColor   = ConvertColor(json.GetJson(JsonManager::FileType::UI)["GLAY_COLOR"]);
	buttonColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["LB_TEXTCOLOR"]);
	textColor	= ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["OPERATION_FONT_SIZE"];



	/*描画*/
	SetFontSize(fontSize);
	DrawBox(dashIcon1.lx, dashIcon1.ly, dashIcon1.rx, dashIcon1.ry, iconColor, TRUE);
	DrawBox(dashIcon3.lx, dashIcon3.ly, dashIcon3.rx, dashIcon3.ry, iconColor, TRUE);
	DrawString(dashText.x, dashText.y, "ダッシュ", textColor);
	DrawString(dashButton1.x, dashButton1.y, "L", buttonColor);
	DrawString(dashButton2.x, dashButton2.y, "+", buttonColor);
	DrawString(dashButton3.x, dashButton3.y, "LB", buttonColor);

}

/// <summary>
/// ボタン文字の描画
/// </summary>
void ButtonUI::DrawButtonText()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Vec2d mainTextPosition;
	Vec2d specialTextPosition;
	Vec2d avoidTextPosition;
	Vec2d blockTextPosition;
	int   mainAttackColor;
	int   specialAttackColor;
	int   avoidColor;
	int   blockColor;
	int   fontSize;

	/*jsonデータの代入*/
	mainTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["X_TEXT_POSITION"]);
	specialTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["Y_TEXT_POSITION"]);
	avoidTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["B_TEXT_POSITION"]);
	blockTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["LB_TEXT_POSITION"]);
	mainAttackColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["X_TEXT_COLOR"]);
	specialAttackColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["Y_TEXT_COLOR"]);
	avoidColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["B_TEXT_COLOR"]);
	blockColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["LB_TEXTCOLOR"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["BUTTON_FONT_SIZE"];

	/*テキストの描画*/
	SetFontSize(fontSize);
	DrawString(mainTextPosition.x, mainTextPosition.y, "X", mainAttackColor);
	DrawString(specialTextPosition.x, specialTextPosition.y, "Y", specialAttackColor);
	DrawString(avoidTextPosition.x, avoidTextPosition.y, "B", avoidColor);
	DrawString(blockTextPosition.x, blockTextPosition.y, "LB", blockColor);
}

/// <summary>
/// 文字の描画
/// </summary>
void ButtonUI::DrawIconText()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Vec2d mainTextPosition;
	Vec2d specialTextPosition;
	Vec2d avoidTextPosition;
	Vec2d blockTextPosition;
	int   fontSize;
	int   textEdgeColor;
	int   textColor;

	/*jsonデータの代入*/
	mainTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	specialTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	avoidTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	blockTextPosition.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	fontSize = json.GetJson(JsonManager::FileType::UI)["ICON_FONT_SIZE"];
	textEdgeColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_EDGE_COLOR"]);
	textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);

	/*テキストの描画*/
	SetFontSize(fontSize);
	DrawString(mainTextPosition.x, mainTextPosition.y, "基本攻撃", textColor, textEdgeColor);
	DrawString(specialTextPosition.x, specialTextPosition.y, "特殊攻撃", textColor, textEdgeColor);
	DrawString(avoidTextPosition.x, avoidTextPosition.y, "回避", textColor, textEdgeColor);
	DrawString(blockTextPosition.x, blockTextPosition.y, "ガード", textColor, textEdgeColor);
}

/// <summary>
/// アイコンの描画
/// </summary>
void ButtonUI::DrawIcon()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Vec2d mainAttackPosition;
	Vec2d specialAttackPosition;
	Vec2d avoidPosition;
	Box   blockPosition;
	int   iconSize;
	int   mainAttackColor;
	int   specialAttackColor;
	int   avoidColor;
	int   blockColor;

	/*jsonデータの代入*/
	mainAttackPosition	 .Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_POSITION"]);
	specialAttackPosition.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_POSITION"]);
	avoidPosition		 .Set(json.GetJson(JsonManager::FileType::UI)["AVOID_POSITION"]);
	blockPosition		 .Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_POSITION"]);
	iconSize			 = json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"];
	mainAttackColor		 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BLUE_COLOR"]);
	specialAttackColor	 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["YELLOW_COLOR"]);
	avoidColor			 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["RED_COLOR"]);
	blockColor			 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["GLAY_COLOR"]);

	/*アイコンの描画*/
	DrawCircle(mainAttackPosition.x	  , mainAttackPosition.y	, iconSize				 , mainAttackColor	 , TRUE);
	DrawCircle(specialAttackPosition.x, specialAttackPosition.y	, this->specialAttackSize, specialAttackColor, TRUE);
	DrawCircle(avoidPosition.x		  , avoidPosition.y			, iconSize				 , avoidColor		 , TRUE);
	DrawBox(blockPosition.lx, blockPosition.ly, blockPosition.rx, blockPosition.ry, blockColor, TRUE);
}

/// <summary>
/// 背景の描画
/// </summary>
void ButtonUI::DrawBackground()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Vec2d mainAttackPosition;
	Vec2d specialAttackPosition;
	Vec2d avoidPosition;
	int   iconSize;
	int   thickness;
	int   backgroundColor;

	/*jsonデータの代入*/
	mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_POSITION"]);
	specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_POSITION"]);
	avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_POSITION"]);
	iconSize				= json.GetJson(JsonManager::FileType::UI)["ICON_SIZE"];
	thickness				= json.GetJson(JsonManager::FileType::UI)["BACKGROUND_THICKNESS"];
	backgroundColor			= ConvertColor(json.GetJson(JsonManager::FileType::UI)["BACKGROUND_COLOR"]);


	/*背景の描画*/
	DrawCircle(mainAttackPosition.x, mainAttackPosition.y, iconSize, backgroundColor, TRUE, thickness);
	DrawCircle(specialAttackPosition.x, specialAttackPosition.y, iconSize, backgroundColor, TRUE, thickness);
	DrawCircle(avoidPosition.x, avoidPosition.y, iconSize, backgroundColor, TRUE, thickness);
}
/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}