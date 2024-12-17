#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "TitleUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
TitleUI::TitleUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	const int TITLE_LOGO = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	const int PRESS_BUTTON_LOGO = asset.GetImage(LoadingAsset::ImageType::PRESS_BUTTON_LOGO);
	this->image.emplace_back(new Image(TITLE_LOGO));
	this->image.emplace_back(new Image(PRESS_BUTTON_LOGO));
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
TitleUI::~TitleUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void TitleUI::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = 0;
		this->image[i]->isAddAlpha = true;
	}
	this->image[static_cast<int>(Type::TITLE)]->SetPosition(json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_DRAW_RECT"]);
	this->image[static_cast<int>(Type::PRESS)]->SetPosition(json.GetJson(JsonManager::FileType::UI)["PRESS_A_DRAW_RECT"]);
	this->isPrevPressButton = false;
	this->isFadeOut = false;
}


/// <summary>
/// 更新
/// </summary>
void TitleUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*pad入力*/
	bool isPressButton = IsPressButton();
	//前にボタンが押されていたか

	/*アルファ処理*/
	int type = static_cast<int>(Type::TITLE);
	int addAlpha = json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"];
	if (!this->isFadeOut)
	{
		//タイトルロゴのアルファが最大アルファ未満だったら
		if (this->image[type]->alpha < Image::MAX_ALPHA)
		{
			//typeロゴのアルファを増加させる
			this->image[type]->FadeIn(addAlpha);
		}
		//違うならtypeはPRESS
		else
		{
			type = static_cast<int>(Type::PRESS);
			addAlpha = json.GetJson(JsonManager::FileType::UI)["PRESS_A_ADD_ALPHA"];
			this->image[type]->FadeInOut(addAlpha);
		}
	}

	if (this->isFadeOut)
	{
		addAlpha = json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ADD_ALPHA"];
		this->image[static_cast<int>(Type::TITLE)]->FadeOut(addAlpha);
	}

	/*ボタンが押されていたらアルファを最大アルファにする*/
	if (isPressButton && !this->image[static_cast<int>(Type::TITLE)]->isAddAlpha)
	{
		this->isFadeOut = true;
		this->image[static_cast<int>(Type::PRESS)]->alpha = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
const void TitleUI::Draw()const
{
	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->Draw();
	}
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool TitleUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	if (this->isFadeOut && this->image[static_cast<int>(Type::TITLE)]->alpha <= 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// ボタンの入力
/// </summary>
bool TitleUI::IsPressButton()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad入力*/
	int pad = input.GetNowPadState();
	bool isPressButton = false;

	/*PRESSロゴが表示されていたら*/
	if (this->image[static_cast<int>(Type::PRESS)]->alpha > 0)
	{
			isPressButton = (pad & PAD_INPUT_4);
			//前にボタン入力がない&今ボタン入力がある
			if (!this->isPrevPressButton && isPressButton)
			{
				this->isPrevPressButton = true;
			}
			//前にボタン入力がある
			else if (this->isPrevPressButton)
			{
				//今ボタン入力がない
				if (!isPressButton)
				{
					this->isPrevPressButton = false;
				}
				isPressButton = false;
			}
	}

	/*何かキーが押されたら、無条件でtrueを返す*/
	if (CheckHitKeyAll()) return true;


	return isPressButton;
}