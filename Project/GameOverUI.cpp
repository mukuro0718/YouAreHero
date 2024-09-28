#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameOverUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverUI::GameOverUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	this->image.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::CONTINUE)));
	this->image.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::END)));

	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverUI::~GameOverUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameOverUI::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->alpha = Image::MAX_ALPHA;
		this->image[i]->isAddAlpha = true;
		this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION"][i]);
	}
	this->isEndExtend = false;
}


/// <summary>
/// 更新
/// </summary>
void GameOverUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	bool isPressButton = IsPressButton();

	SetType();

	/*基底の大きさまで画像を拡大*/
	if (!this->isEndExtend)
	{
		int endCount = 0;
		int index = this->image.size();
		for (int i = 0; i < index; i++)
		{
			bool isEnd = this->image[i]->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_POSITION"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_ADD_VALUE"]);
			if (isEnd)
			{
				endCount++;
			}
		}
		if (endCount == index)
		{
			this->isEndExtend = true;
		}
	}
	else
	{
		/*拡大し終わったら、選択中のものを拡大縮小させる*/
		for (int i = 0; i < this->image.size(); i++)
		{
			if (this->type == i)
			{
				this->image[i]->ScalingGraph(json.GetJson(JsonManager::FileType::UI)["RESULT_IMAGE_TARGET_DRAW_RECT"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_REDUCED_POSITION"][i], json.GetJson(JsonManager::FileType::UI)["GAME_OVER_ADD_VALUE"]);
			}
			else
			{
				this->image[i]->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_POSITION"][i]);
			}
		}
		/*ボタンが押されていたらリザルト終了*/
		if (isPressButton)
		{
			//typeがContinueだったらtrue/Endだったらfalseを返す
			this->isEnd = true;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameOverUI::Draw()const
{
	for (int i = 0; i < this->image.size(); i++)
	{
		this->image[i]->Draw();
	}
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool GameOverUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	return this->isEnd;
}

/// <summary>
/// ゲームオーバー時、アイコンの種類を設定
/// </summary>
void GameOverUI::SetType()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	const int X_BUF = input.GetLStickState().XBuf;

	/*右*/
	if (X_BUF > 0)
	{
		this->type++;
		if (this->type >= static_cast<int>(ImageType::END))
		{
			this->type = static_cast<int>(ImageType::END);
		}
	}
	/*左*/
	else if (X_BUF < 0)
	{
		this->type--;
		if (this->type <= static_cast<int>(ImageType::CONTINUE))
		{
			this->type = static_cast<int>(ImageType::CONTINUE);
		}
	}
}

/// <summary>
/// ボタンの入力
/// </summary>
bool GameOverUI::IsPressButton()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad入力*/
	int pad = input.GetPadState();
	bool isPressButton = false;

	/*windowが移動していない/拡大もしていなければ*/
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

	return isPressButton;
}


const bool GameOverUI::IsContinue()const
{
	return this->isContinue;
}