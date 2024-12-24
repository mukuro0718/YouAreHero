#include "DxLib.h"
#include "UseJson.h"
#include "FPSController.h"
#include "Debug.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
FPSController::FPSController()
	: fps(0.0f)
	, startTime(0)
	, targetFPS(0)
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
FPSController::~FPSController()
{

}

/// <summary>
/// 初期化
/// </summary>
void FPSController::Initialize()
{
	/*Jsonマネージャーのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);
	this->fps		= 0.0f;
	this->startTime = 0;
	this->timeScale = this->NORMAL_TIME_SCALE;
	this->frameCount = 0;
	this->lastTime = GetNowCount();
}

/// <summary>
/// 開始時間の計算
/// </summary>
void FPSController::CalcStartTime()
{
	this->startTime = GetNowCount();
}
/// <summary>
/// FPSの更新
/// </summary>
void FPSController::Update()
{
	/*Jsonマネージャーのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*目標FPS*/
	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);

	/*現在のフレーム時間*/
	int frameTime = GetNowCount() - this->startTime;

	/*目標のフレーム時間*/
	int targetFrameTime = this->timeScale / targetFPS;

	/*現在のフレーム時間が目標のフレーム時間よりも小さければ*/
	if (frameTime < targetFrameTime)
	{
		//目標のフレーム時間になるまで待機する
		Sleep(targetFrameTime - frameTime);
	}

	// FPSの計算
	this->frameCount++;
	int currentTime = GetNowCount();
	// 1秒ごとに更新
	if (currentTime - this->lastTime >= this->timeScale)
	{ 
		this->fps = (this->frameCount * static_cast<double>(this->timeScale)) / (currentTime - this->lastTime);
		this->lastTime = currentTime;
		this->frameCount = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
const void FPSController::Draw()const
{
	/*クラスインスタンスの取得*/
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.IsShowDebugInfo(Debug::ItemType::FPS))
	{
		printfDx("FPS:%f\n", this->fps);
	}
}
