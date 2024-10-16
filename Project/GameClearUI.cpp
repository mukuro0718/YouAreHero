#include <DxLib.h>
#include<iostream>
#include<fstream>
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
#include "Timer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameClearUI::GameClearUI()
	: imageHandle	(-1)
	, fontHandle	(-1)
	, alpha			(0)
	, nowRanking	(-1)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*画像クラスインスタンスの作成*/
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::BACK_GROUND);

	/*fontHandleの取得*/
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_80_32);

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


	this->isEnd				= false;
	this->isPrevPressButton = false;
	this->alpha				= 0;
	this->nowRanking		= -1;

	/*ランキングの更新*/
	CalcRanking();
}

/// <summary>
/// 更新
/// </summary>
void GameClearUI::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();

	/*拡大が終了していなければ拡大して早期リターン*/
	if (this->alpha < json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"])
	{
		this->alpha += json.GetJson(JsonManager::FileType::UI)["GAME_ADD_ALPHA"];
		return;
	}

	/*ボタン入力*/
	bool isPressButton = IsPressButton();
	
	if (isPressButton)
	{
		this->isEnd = true;
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameClearUI::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& timer = Singleton<Timer>	::GetInstance();

	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	vector<int> table = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TABLE_POSITION"];
	DrawExtendGraph(table[0], table[1], table[2], table[3], this->imageHandle, TRUE);

	/*クリアタイムの表示*/
	vector<int> clearTimePosition = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TIME_POSITION"];
	int second = timer.GetNowTime() / 60;
	int minute = second / 60;
	second -= minute * 60;
	DrawFormatStringToHandle(clearTimePosition[0], clearTimePosition[1], this->TEXT_COLOR, this->fontHandle, "討伐時間 %d分%d秒", minute, second);

	/*順位の表示*/
	vector<int> rankingPosition = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_POSITION"];
	if (this->nowRanking == -1)
	{
		DrawStringToHandle(rankingPosition[0], rankingPosition[1],"討伐順位 不明" ,this->TEXT_COLOR, this->fontHandle);
	}
	else
	{
		DrawFormatStringToHandle(rankingPosition[0], rankingPosition[1], this->TEXT_COLOR, this->fontHandle, "討伐順位 %d", this->nowRanking + 1);
	}

	vector<int> position = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_BACK_TITLE_POSITION"];
	DrawStringToHandle(position[0], position[1], "Bでタイトルに戻る", this->TEXT_COLOR, this->fontHandle);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, json.GetJson(JsonManager::FileType::UI)["GAME_MAX_ALPHA"]);

}

void GameClearUI::CalcRanking()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	auto& timer = Singleton<Timer>	::GetInstance();

	int clearTime = timer.GetNowTime();//ここにクリアタイムを入れる


	//ファイルを読み込んで内容を画面に表示する
	string fileName = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_FILE_NAME"];

	ifstream ifs(fileName.c_str());
	if (ifs.good())
	{
		nlohmann::json nowJson;
		ifs >> nowJson;

		//読み込んだデータをそれぞれの変数に代入する
		vector<int>	ranking = nowJson["RANKING"];

		int index = ranking.size();
		//クリアタイムの確認
		for (int i = 0; i < index; i++)
		{
			//ランクインするなら、順位を保持し、Rankingを更新する
			if (ranking[i] >= clearTime)
			{
				this->nowRanking = i;
				ranking.insert(ranking.begin() + i, clearTime);
				break;
			}
		}
		//もしランクインしていないときに、インデックスが最大未満だったら追加する
		if (this->nowRanking == -1 && index != json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_MAX_RANKING_USER"])
		{
			ranking.emplace_back(clearTime);
			this->nowRanking = index;
		}
		//ファイルに書き込む内容
		nowJson =
		{
			{"RANKING",ranking },
		};
		//作成したファイルに内容を書き込む
		ofstream writing_file;
		writing_file.open(fileName, ios::out);
		writing_file << nowJson.dump() << endl;
		writing_file.close();

	}
	//ファイルがない場合は新しく作る
	else
	{
		string indexName = json.GetJson(JsonManager::FileType::UI)["GAME_CLEAR_RANKING_INDEX_NAME"];
		vector<int> value;
		value.emplace_back(clearTime);
		value.emplace_back(10000);
		nlohmann::json newJson = 
		{ 
			{indexName.c_str(),value},
		};
		//作成したファイルに内容を書き込む
		ofstream writing_file;
		writing_file.open(fileName, ios::out);
		writing_file << newJson.dump() << endl;
		writing_file.close();
		this->nowRanking = 1;
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

