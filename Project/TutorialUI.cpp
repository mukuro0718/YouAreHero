#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "TutorialUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
TutorialUI::TutorialUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	const int WINDOW = asset.GetImage(LoadingAsset::ImageType::WINDOW);
	this->window = new Image(WINDOW);
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::MINTYO_50_32));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::MINTYO_50_32));

	/*初期化*/
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
	/*シングルトンクラスのインスタンスを取得*/
	auto& json   = Singleton<JsonManager>	 ::GetInstance();

	this->window->SetPosition(json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_FIRST_DRAW_RECT"]);
	this->window->alpha		 = Image::MAX_ALPHA;
	this->window->isAddAlpha = false;
	this->isExtendWindow	 = true;
	this->isReducedWindow	 = false;
	this->isEnd				 = false;
	this->isPrevPressButton  = false;
}


/// <summary>
/// 更新
/// </summary>
void TutorialUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	bool isPressButton = IsPressButton();

	if (isPressButton)
	{
		this->isReducedWindow = true;
	}

	/*windowの拡大・縮小*/
	ExtendWindow();
	ReducedWindow();
}

/// <summary>
/// 描画
/// </summary>
const void TutorialUI::Draw()const
{
	vector<int> position = this->window->position;
	DrawExtendGraph(position[0], position[1], position[2], position[3], this->window->imageHandle, TRUE);
	/*拡大も縮小もしていなければ*/
	if (!this->isExtendWindow && !this->isReducedWindow)
	{
		//シングルトンクラスのインスタンスを取得
		auto& json = Singleton<JsonManager>::GetInstance();
		{
			string drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TITLE_TEXT"];
			vector<int> fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TITLE_POSITION"];
			DrawStringToHandle(fontPosition[0], fontPosition[1], drawText.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
		}
		{
			vector<string>	drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TEXT"];
			vector<int>		fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_FONT_POSITION"];
			int				fontSize = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_FONT_SIZE"];
			//文字の描画
			for (int i = 0; i < drawText.size(); i++)
			{
				DrawStringToHandle(fontPosition[0], fontPosition[1], drawText[i].c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
				fontPosition[1] += fontSize;
			}
			fontPosition.clear();
			drawText.clear();
		}
		{
			string drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_CLOSE_WINDOW_TEXT"];
			vector<int> fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_CLOSE_WINDOW_POSITION"];
			DrawStringToHandle(fontPosition[0], fontPosition[1], drawText.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::SUB)]);
		}
	}
}

/// <summary>
/// シーンを終了するか
/// </summary>
const bool TutorialUI::IsEnd()const
{
	return this->isEnd;
}

/// <summary>
/// オフセットを追加した値を返す
/// </summary>
std::vector<int> TutorialUI::ReturnAddOffset(const std::vector<int> _position, const std::vector<int> _offset)
{
	std::vector<int> out = _position;

	for (int i = 0; i < out.size(); i++)
	{
		out[i] += _offset[i];
	}
	return out;
}


/// <summary>
/// windowの拡大
/// </summary>
void TutorialUI::ExtendWindow()
{
	if (this->isExtendWindow)
	{
		/*シングルトンクラスのインスタンスを取得*/
		auto& json = Singleton<JsonManager>	::GetInstance();
		std::vector<int> targetPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_EXTEND_DRAW_RECT"];
		std::vector<int> addValue = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_ADD_VALUE"];

		/*拡大処理*/
		bool isEndExtend = false;
		isEndExtend = this->window->ExtendGraph(targetPosition, addValue);
		targetPosition = ReturnAddOffset(targetPosition, json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_OFFSET"]);
		/*拡大していたら*/
		if (isEndExtend)
		{
			this->isExtendWindow = false;
		}
	}
}

/// <summary>
/// windowの縮小
/// </summary>
void TutorialUI::ReducedWindow()
{
	if (this->isReducedWindow)
	{
		/*シングルトンクラスのインスタンスを取得*/
		auto& json = Singleton<JsonManager>	::GetInstance();
		std::vector<int> targetPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_FIRST_DRAW_RECT"];
		std::vector<int> decreaseValue = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_DECREASE_VALUE"];

		/*縮小処理*/
		bool isEndReduced = false;
		isEndReduced = this->window->ExtendGraph(targetPosition, decreaseValue);
		targetPosition = ReturnAddOffset(targetPosition, json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_OFFSET"]);
		
		/*縮小していたら*/
		if (isEndReduced)
		{
			this->isReducedWindow = false;
			this->isEnd = true;
		}
	}
}

/// <summary>
/// ボタンの入力
/// </summary>
bool TutorialUI::IsPressButton()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad入力*/
	int pad = input.GetPadState();
	bool isPressButton = false;

	/*windowが移動していない/拡大もしていなければ*/
	if (!this->isExtendWindow && !this->isReducedWindow)
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

	return isPressButton;
}