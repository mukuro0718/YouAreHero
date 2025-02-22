#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "CreditUI.h"
#include "LoadingAsset.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CreditUI::CreditUI()
	: isOpenCredit(false)
	, yOffset (0)
	, ADD_Y_OFFSET(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["CREDIT_ADD_Y_OFFSET"])
	, TEXT_FONT_SIZE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["CREDIT_TEXT_FONT_SIZE"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::SELECT_QUEST);
}

/// <summary>
/// デストラクタ
/// </summary>
CreditUI::~CreditUI()
{

}

/// <summary>
/// 更新
/// </summary>
void CreditUI::Update()
{
	/*クレジットを開くかの処理*/
	auto& input = Singleton<InputManager>::GetInstance();
	int nowPad = input.GetNowPadState();
	//開いていないときに,Startボタンが押されたら開く
	if (!this->isOpenCredit)
	{
		if (nowPad & InputManager::PAD_START)
		{
			this->isOpenCredit = true;
		}
	}
	//開いている時に、Bボタンが押されたら閉じる
	else
	{
		if (nowPad & InputManager::PAD_B)
		{
			this->isOpenCredit = false;
		}
	}
	/*クレジットを開いていなければ早期リターン*/

	/*左スティックの入力状況で、画面をスクロールする（必要なら）*/
}

/// <summary>
/// 描画
/// </summary>
const void CreditUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();
	vector<int> position = json.GetJson(JsonManager::FileType::UI)["CREDIT_POSITION"];
	/*クレジットを開いていなければ*/
	if (!this->isOpenCredit)
	{
		DrawStringToHandle(position[0], position[1], "START : OPEN CREDIT", this->FONT_COLOR, this->fontHandle);
	}
	else
	{
		/*背景の描画*/
		DrawBox(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->BACK_COLOR, TRUE);

		/*クレジットの表示*/
		vector<string> creditText = json.GetJson(JsonManager::FileType::UI)["CREDIT_TEXT"];
		position[1] += this->yOffset;
		for (int i = 0; i < creditText.size(); i++)
		{
			DrawStringToHandle(position[0], position[1], creditText[i].c_str(), this->FONT_COLOR, this->fontHandle);
			position[1] += this->TEXT_FONT_SIZE;
		}
	}
}