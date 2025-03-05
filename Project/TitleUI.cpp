#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "CreditUI.h"
#include "TitleUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
TitleUI::TitleUI()
{
	/*画像クラスインスタンスの作成*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->titleLogo.handle = asset.GetImage(LoadingAsset::ImageType::TITLE_LOGO);
	this->pressLogo.handle = asset.GetImage(LoadingAsset::ImageType::PRESS_BUTTON_LOGO);

	/*メンバ変数の初期化*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	this->titleLogo.x				= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_POSITION"][0];
	this->titleLogo.y				= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_POSITION"][1];
	this->titleLogo.rate			= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_RATE"];
	this->titleLogo.alphaIncrease	= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ALPHA_INCREASE"];
	this->titleLogo.alphaReduction	= json.GetJson(JsonManager::FileType::UI)["TITLE_LOGO_ALPHA_REDUCTION"];
	this->titleLogo.alpha			= 0.0f;
	this->titleLogo.angle			= 0.0f;
	this->pressLogo.x				= json.GetJson(JsonManager::FileType::UI)["PRESS_A_POSITION"][0];
	this->pressLogo.y				= json.GetJson(JsonManager::FileType::UI)["PRESS_A_POSITION"][1];
	this->pressLogo.rate			= json.GetJson(JsonManager::FileType::UI)["PRESS_A_RATE"];
	this->pressLogo.alphaIncrease	= json.GetJson(JsonManager::FileType::UI)["PRESS_A_ALPHA_INCREASE"];
	this->pressLogo.alphaReduction	= json.GetJson(JsonManager::FileType::UI)["PRESS_A_ALPHA_REDUCTION"];
	this->pressLogo.alpha			= 0.0f;
	this->pressLogo.angle			= 0.0f;

	this->credit = new CreditUI();

	/*初期化*/
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
	this->titleLogo.alpha			= 0.0f;
	this->pressLogo.alpha			= 0.0f;
	this->isFadeIn					= true;
	this->alphaForTransition		= this->MAX_ALPHA;
	this->isTransition				= true;
	this->isEndFadeInForTransition	= false;
}


/// <summary>
/// 更新
/// </summary>
void TitleUI::Update()
{
	/*画面遷移用のアルファ値が残っていたら*/
	if (this->isTransition)
	{
		//遷移用のフェードインが終わって居なかったら
		if (!this->isEndFadeInForTransition)
		{
			this->alphaForTransition -= this->ALPHA_REDUCTION;
			if (this->alphaForTransition <= 0)
			{
				this->isEndFadeInForTransition = true;
				this->isTransition = false;
			}
		}
		else
		{
			this->alphaForTransition += this->ALPHA_INCREASE;
		}
	}
	else
	{
		/*クレジットの更新処理*/
		this->credit->Update();

		/*クレジットを開いていたらその他処理は行わない*/
		if (this->credit->GetIsOpen())return;

		/*pad入力*/
		bool isPressButton = IsPressButton();

		/*アルファ処理*/
		//タイトルロゴのアルファが最大アルファ未満だったらアルファを増加させる
		if (this->titleLogo.alpha < this->MAX_ALPHA)
		{
			this->titleLogo.alpha += this->titleLogo.alphaIncrease;
		}
		//それ以外ならプレスボタンをフェードインアウトする
		else
		{
			if (this->isFadeIn)
			{
				this->pressLogo.alpha += this->pressLogo.alphaIncrease;
				if (this->pressLogo.alpha >= this->MAX_ALPHA)
				{
					this->isFadeIn = false;
				}
			}
			else
			{
				this->pressLogo.alpha -= this->pressLogo.alphaReduction;
				if (this->pressLogo.alpha <= 0)
				{
					this->isFadeIn = true;
				}
			}
			//ボタンが押されていたらアルファを最大アルファにする
			if (isPressButton)
			{
				auto& sound = Singleton<SoundManager>::GetInstance();
				sound.OnIsPlayEffect(SoundManager::EffectType::CONFIRMATION_SOUND);
				this->isTransition = true;
				this->pressLogo.alpha = 0;
			}
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void TitleUI::Draw()const
{
	/*タイトルロゴの描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,this->titleLogo.alpha);
	DrawRotaGraph(this->titleLogo.x, this->titleLogo.y, this->titleLogo.rate, this->titleLogo.angle, this->titleLogo.handle, TRUE);;
	/*プレスボタンの描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->pressLogo.alpha);
	DrawRotaGraph(this->pressLogo.x, this->pressLogo.y, this->pressLogo.rate, this->pressLogo.angle, this->pressLogo.handle, TRUE);;
	/*シーン遷移の描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alphaForTransition);
	DrawBox(0, 0, this->MAX_X, this->MAX_Y, 0, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
	/*クレジットの表示*/
	this->credit->Draw();
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool TitleUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	if (this->isEndFadeInForTransition && this->alphaForTransition >= this->MAX_ALPHA)
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
	bool isPressButton = false;

	/*PRESSロゴが表示されていたら*/
	if (this->pressLogo.alpha > 0)
	{
			isPressButton = input.GetNowPad(InputManager::PAD_A);
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
	//if (CheckHitKeyAll()) return true;


	return isPressButton;
}