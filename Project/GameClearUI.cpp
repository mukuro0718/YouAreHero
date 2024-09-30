#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "GameClearUI.h"
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
GameClearUI::GameClearUI()
	: window			(nullptr)
	, isPrevPressButton	(false)
	, isEnd				(false)
	, isEndExtend		(false)
	, isRedused			(false)
	, starIndex			(0)
	, frameCount		(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	this->window = new Image(asset.GetImage(LoadingAsset::ImageType::WINDOW));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));
	this->starFrame.emplace_back(new Image(asset.GetImage(LoadingAsset::ImageType::NONE_STAR)));

	/*fontHandleの取得*/
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_100_64));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::BAT_30_64));

	//Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameClearUI::~GameClearUI()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameClearUI::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	SetStarHandle();

	this->window->alpha = Image::MAX_ALPHA;
	this->window->isAddAlpha = true;
	this->window->SetPosition(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_FIRST_POSITION"]);

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_POSITION"];
	for (int i = 0; i < this->star.size(); i++)
	{
		this->star[i]->alpha = Image::MAX_ALPHA;
		this->star[i]->isAddAlpha = true;
		this->star[i]->SetPosition(position);
		this->starFrame[i]->alpha = Image::MAX_ALPHA;
		this->starFrame[i]->isAddAlpha = true;
		this->starFrame[i]->SetPosition(position);
		position = AddPositionOffset(position, json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_OFFSET"]);
	}
	this->isPrevPressButton = false;
	this->isEnd = false;
	this->isEndExtend = false;
	this->isRedused = false;
	this->starIndex = false;
	this->frameCount = 0;
}

/// <summary>
/// 更新
/// </summary>
void GameClearUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*ボタン入力*/
	bool isPressButton = IsPressButton();

	/*テキストインデックスの追加*/
	AddStarIndex();

	/*windowの拡大が終わっているか*/
	if (!this->isEndExtend)
	{
		this->isEndExtend = this->window->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_TARGET_POSITION"], json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_ADD_VALUE"]);
		return;
	}


	if (isPressButton && this->starIndex == this->star.size())
	{
		this->isRedused = true;
	}

	if (this->isRedused)
	{
		this->isEnd = this->window->ExtendGraph(json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_FIRST_POSITION"], json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WINDOW_ADD_VALUE"]);
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameClearUI::Draw()const
{
	/*windowの描画*/
	this->window->Draw();

	/*windowの拡大が終了していなければ早期リターン*/
	if (!this->isEndExtend)return;

	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*星の描画*/
	for (int i = 0; i < this->starFrame.size(); i++)
	{
		this->starFrame[i]->Draw();
	}
	for (int i = 0; i < this->starIndex; i++)
	{
		this->star[i]->Draw();
	}


	/*テキストの描画*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_HEADER_POSITION"];
		string text = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_HEADER"];
		DrawStringToHandle(position[0], position[1], text.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::HEADER)]);
	}
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_MAIN_POSITION"];
		string text = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TEXT_MAIN"];
		for (int i = 0; i < text.size(); i++)
		{
			DrawStringToHandle(position[0], position[1], text.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
		}
	}
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool GameClearUI::IsEnd()const
{
	/*PRESSのアルファが変動していたら表示している*/
	return this->isEnd;
}



/// <summary>
/// ボタンの入力
/// </summary>
bool GameClearUI::IsPressButton()
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

/// <summary>
/// テキストインデックスの追加
/// </summary>
void GameClearUI::AddStarIndex()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*拡大が終了していなければ早期リターン*/
	if (!this->isEndExtend)return;
	/*インデックスが定数を超えていたら早期リターン*/
	if (this->starIndex >= json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_STAR_NUM"])return;

	this->frameCount++;
	/*フレームカウントが定数を超えていたら*/
	if (this->frameCount >= json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_ADD_STAR_INDEX_INTERVAL"])
	{
		this->frameCount = 0;
		this->starIndex++;
	}
}


void GameClearUI::SetStarHandle()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	int goldStar = asset.GetImage(LoadingAsset::ImageType::GOLD_STAR);
	int silverStar = asset.GetImage(LoadingAsset::ImageType::SILVER_STAR);
	int time = 0;//経過時間
	int orbNum = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"] - player.GetHealOrbNum();//オーブの残り
	int continueCount = -1;
	vector<int> judge = { time,orbNum,continueCount };
	vector<int> goldLine = json.GetJson(JsonManager::FileType::UI)["GOLD_LINE"];
	vector<int> silverLine = json.GetJson(JsonManager::FileType::UI)["SILVER_LINE"];


	this->star.clear();
	for (int i = 0; i < goldLine.size(); i++)
	{
		/*時間がシルバーラインを超えていたら*/
		if (judge[i] <= silverLine[i])
		{
			//ゴールドライン
			if (judge[i] <= goldLine[i])
			{
				this->star.emplace_back(new Image(goldStar));
			}
			else
			{
				this->star.emplace_back(new Image(silverStar));
			}
		}
		else
		{
			this->star.emplace_back(new Image(-1));
		}
	}
}


vector<int> GameClearUI::AddPositionOffset(const vector<int> _position,const vector<int> _offset)
{
	vector<int> out = _position;
	for (int i = 0; i < out.size(); i++)
	{
		out[i] += _offset[i];
	}
	return out;
}