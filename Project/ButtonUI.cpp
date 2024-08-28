#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "ButtonUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ButtonUI::ButtonUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->b = asset.GetImage(LoadingAsset::ImageType::B);
	this->x = asset.GetImage(LoadingAsset::ImageType::X);
	this->y = asset.GetImage(LoadingAsset::ImageType::Y);
	this->lb = asset.GetImage(LoadingAsset::ImageType::LB);
	this->ls = asset.GetImage(LoadingAsset::ImageType::LS);
	this->lt = asset.GetImage(LoadingAsset::ImageType::LT);
	this->rb = asset.GetImage(LoadingAsset::ImageType::RB);
	this->rs = asset.GetImage(LoadingAsset::ImageType::RS);
	this->iconFont = asset.GetFont(LoadingAsset::FontType::ICON_UI);
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

	/*変数の準備*/
	Box xBox,yBox, bBox, ltBox,lbBox,lsBox;
	/*jsonデータの代入*/
	xBox.Set(json.GetJson(JsonManager::FileType::UI)["X_BOX"]);
	yBox.Set(json.GetJson(JsonManager::FileType::UI)["Y_BOX"]);
	bBox.Set(json.GetJson(JsonManager::FileType::UI)["B_BOX"]);
	ltBox.Set(json.GetJson(JsonManager::FileType::UI)["LT_BOX"]);
	lbBox.Set(json.GetJson(JsonManager::FileType::UI)["LB_BOX"]);
	lsBox.Set(json.GetJson(JsonManager::FileType::UI)["LS_BOX"]);

	/*アイコンの描画*/
	DrawExtendGraph(xBox.lx, xBox.ly, xBox.rx, xBox.ry, this->x, TRUE);
	DrawExtendGraph(yBox.lx, yBox.ly, yBox.rx, yBox.ry, this->y, TRUE);
	DrawExtendGraph(bBox.lx, bBox.ly, bBox.rx, bBox.ry, this->b, TRUE);
	DrawExtendGraph(ltBox.lx, ltBox.ly, ltBox.rx, ltBox.ry, this->lt, TRUE);
	DrawExtendGraph(lbBox.lx, lbBox.ly, lbBox.rx, lbBox.ry, this->lb, TRUE);
	DrawExtendGraph(lsBox.lx, lsBox.ly, lsBox.rx, lsBox.ry, this->ls, TRUE);
}

/// <summary>
/// 背景の描画
/// </summary>
void ButtonUI::DrawFont()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*変数の準備*/
	Vec2d mainAttackPosition,specialAttackPosition,avoidPosition,blockPosition,dashPosition;
	int   textColor;

	/*jsonデータの代入*/
	mainAttackPosition		.Set(json.GetJson(JsonManager::FileType::UI)["MAIN_ATTACK_TEXT_POSITION"]);
	specialAttackPosition	.Set(json.GetJson(JsonManager::FileType::UI)["SPECIAL_ATTACK_TEXT_POSITION"]);
	avoidPosition			.Set(json.GetJson(JsonManager::FileType::UI)["AVOID_TEXT_POSITION"]);
	blockPosition			.Set(json.GetJson(JsonManager::FileType::UI)["BLOCK_TEXT_POSITION"]);
	dashPosition			.Set(json.GetJson(JsonManager::FileType::UI)["DASH_TEXT_POSITION"]);
	textColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["TEXT_COLOR"]);


	/*背景の描画*/
	DrawStringToHandle(mainAttackPosition.x	  , mainAttackPosition.y	, "攻撃"	 , textColor, this->iconFont);
	DrawStringToHandle(specialAttackPosition.x, specialAttackPosition.y	, "スキル"  , textColor, this->iconFont);
	DrawStringToHandle(avoidPosition.x		  , avoidPosition.y			, "回避"	 , textColor, this->iconFont);
	DrawStringToHandle(blockPosition.x		  , blockPosition.y			, "ガード"	 , textColor, this->iconFont);
	DrawStringToHandle(dashPosition.x		  , dashPosition.y			, "ダッシュ", textColor, this->iconFont);
}
/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int ButtonUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}