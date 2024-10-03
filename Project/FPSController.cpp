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
	auto& json = Singleton<JsonManager>::GetInstance();

	this->targetFPS = static_cast<int>(json.GetJson(JsonManager::FileType::FPS_CONTROLLER)["TARGET_FPS"]);
	this->fps		= 0.0f;
	this->startTime = 0;
	this->count		= 0;
	this->isDebug	= false;
	this->timeScale = this->NORMAL_TIME_SCALE;
}


void FPSController::CalcStartTime()
{
	if (this->count == 0)
	{
		this->startTime = GetNowCount();
	}
}
/// <summary>
/// FPSの平均
/// </summary>
void FPSController::Average()
{
	UpdateTargetFPS();

	/*Jsonマネージャーのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*カウントが目標FPSになったら*/
	if (this->count == this->targetFPS)
	{
		//現在の時刻を取得
		int nowTime = GetNowCount();
		//FPS計算
		this->fps = static_cast<double>(this->timeScale / (nowTime - this->startTime) / this->targetFPS);
		//初期化
		this->startTime = 0;
		this->count = 0;
	}
	else
	{
		this->count++;
	}
}
const void FPSController::Draw()const
{
	/*クラスインスタンスの取得*/
	auto& debug = Singleton<Debug>::GetInstance();

	if (debug.CheckFPSFlag())
	{
		printfDx("FPS:%f\n", this->fps);
		printfDx("P:+ M:- TARGET_FPS:%d\n", this->targetFPS);
	}
}

/// <summary>
/// targetFPSの更新
/// </summary>
void FPSController::UpdateTargetFPS()
{
	/*クラスインスタンスの取得*/
	auto& debug = Singleton<Debug>::GetInstance();
	auto& input = Singleton<InputManager>::GetInstance();

	if (debug.CheckFPSFlag())
	{
		if (CheckHitKey(KEY_INPUT_P))
		{
			this->targetFPS++;
			if (this->targetFPS >= 60)
			{
				this->targetFPS = 60;
			}
		}
		if (CheckHitKey(KEY_INPUT_M))
		{
			this->targetFPS--;
			if (this->targetFPS <= 0)
			{
				this->targetFPS = 0;
			}
		}
	}
}

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
	int waitTime = this->count * this->timeScale / static_cast<int>(targetFPS) - elapsedTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}	
}
