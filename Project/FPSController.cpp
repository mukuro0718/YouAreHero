#include <DxLib.h>
#include "UseJson.h"
#include "FPSController.h"

/// <summary>
/// コンストラクタ
/// </summary>
FPSController::FPSController()
	: fps(0.0f)
	, startTime(0)
	, count(0)
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
	auto& json = JsonManager::GetInstance();

	fps = 0.0f;
	startTime = 0;
	count = 0;
	targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);
}

/// <summary>
/// FPSの平均
/// </summary>
void FPSController::Average()
{
	//UpdateTargetFPS();

	/*Jsonマネージャーのインスタンスの取得*/
	auto& json = JsonManager::GetInstance();

	/*カウントが０だったらスタートタイムを初期化*/
	if (this->count == 0)
	{
		this->startTime = GetNowCount();
	}

	/*カウントが目標FPSになったら*/
	if (this->count == targetFPS)
	{
		//現在の時刻を取得
		int nowTime = GetNowCount();
		//FPS計算
		this->fps = 1000.0 / (static_cast<double>((nowTime - this->startTime)) / targetFPS);
		//初期化
		this->startTime = 0;
		this->count = 0;
	}
	else
	{
		this->count++;
	}
}

///// <summary>
///// targetFPSの更新
///// </summary>
//void FPSController::UpdateTargetFPS()
//{
//	/*クラスインスタンスの取得*/
//	auto& debug = DebugMode::GetInstance();
//	auto& input = InputManager::GetInstance();
//
//	if (debug.GetIsShowMenu())
//	{
//		if (CheckHitKey(KEY_INPUT_P))
//		{
//			targetFps++;
//			if (targetFps >= 60)
//			{
//				targetFps = 60;
//			}
//		}
//		if (CheckHitKey(KEY_INPUT_M))
//		{
//			targetFps--;
//			if (targetFps <= 0)
//			{
//				targetFps = 0;
//			}
//		}
//		printfDx("P:+ M:- fps:%d", targetFps);
//	}
//}
//
/// <summary>
/// 目標FPSになるよう待機
/// </summary>
void FPSController::Wait()
{
	/*Jsonマネージャーのインスタンスの取得*/
	auto& json = JsonManager::GetInstance();

	/*現在の経過時間*/
	int elapsedTime = GetNowCount() - this->startTime;
	
	/*待機時間*/
	int waitTime = this->count * 1000 / static_cast<int>(targetFPS) - elapsedTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
}
