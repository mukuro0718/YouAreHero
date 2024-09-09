#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneUI.h"
#include "TutorialUI.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
TutorialUI::TutorialUI()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
TutorialUI::~TutorialUI()
{

}

/// <summary>
/// 初期化
/// </summary>
void TutorialUI::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();


	this->logo.SetPosition(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_DRAW_RECT"]);
	this->logo.alpha = 0;
	this->logo.imageHandle = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	this->logo.isAddAlpha = true;

	this->pressA.SetPosition(json.GetJson(JsonManager::FileType::UI)["PRESS_A_DRAW_RECT"]);
	this->pressA.alpha = 0;
	this->pressA.imageHandle = asset.GetImage(LoadingAsset::ImageType::PRESS_A);
	this->pressA.isAddAlpha = true;
}

/// <summary>
/// 更新
/// </summary>
void TutorialUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*アルファの更新*/
	this->logo.AddAlpha(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"]);
	this->pressA.FlashingAlpha(json.GetJson(JsonManager::FileType::UI)["PRESS_A_ADD_ALPHA"]);
}

/// <summary>
/// 描画
/// </summary>
const void TutorialUI::Draw() const
{
	this->logo.Draw();
	this->pressA.Draw();
}