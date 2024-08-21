#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerStormMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStormMagic::PlayerStormMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, frameCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStormMagic::~PlayerStormMagic()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerStormMagic::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*定数型の初期値を作成*/
	const float POSITION_OFFSET   = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_POSITON_OFFSET"];
	const float POSITION_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_POSITON_Y_OFFSET"];
	const float RADIUS			  = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_RADIUS"];
	/*変数を初期化*/
	this->moveVector	=  VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector	=  VNorm(this->moveVector);
	this->position		=  player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->endPosition	=  this->position;
	this->endPosition.y += POSITION_OFFSET_Y;
	this->radius		=  RADIUS;
	this->frameCount	=  0;
	this->isInitialize	=  true;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStormMagic::Update()
{
	/*もし魔法が開始していなかったら早期リターン*/
	if (!this->isStart)return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*初期化していなかったら*/
	if (!this->isInitialize)
	{
		Initialize();
	}

	/*フレーム計測*/
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_INTERVAL"];
	this->frameCount++;

	if (this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isStart = false;
		this->isInitialize = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerStormMagic::Draw()const
{
	/*移動していなかったらreturnを返す*/
	if (!this->isStart)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}