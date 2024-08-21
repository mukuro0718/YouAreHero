#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerIceMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerIceMagic::PlayerIceMagic()
	: firstPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, isBooting(false)
	, frameCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerIceMagic::~PlayerIceMagic()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerIceMagic::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*定数型の初期値を作成*/
	const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_POSITON_OFFSET"];
	const float RADIUS			= json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_RADIUS"];
	const float VELOCITY		= json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_VELOCITY"];
	/*変数を初期化*/
	this->moveVector = VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector = VNorm(this->moveVector);
	this->position = player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->firstPosition = this->position;
	this->radius = RADIUS;
	this->velocity = VELOCITY;
	this->moveVector = VScale(this->moveVector, this->velocity);
	this->isInitialize = true;
}

/// <summary>
/// 更新
/// </summary>
void PlayerIceMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_INTERVAL"];
	this->frameCount++;

	/*魔法が起動していないときフレームが最大値以上だったら魔法を起動する*/
	if (!this->isBooting && this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isBooting = true;
		this->frameCount = 0;
	}

	/*魔法が起動していなかったら早期リターン*/
	if (!this->isBooting)return;

	/*座標の更新*/
	this->position = this->position + this->moveVector;

	/*初期座標との距離が定数以上だったら各フラグを下す*/
	VECTOR		firstToNowPositionVector = this->position - this->firstPosition;
	float		firstToNowPositionVectorSize = VSize(firstToNowPositionVector);
	const float MAX_MOVE_RANGE = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_MAX_MOVE_RANGE"];
	if (firstToNowPositionVectorSize >= MAX_MOVE_RANGE)
	{
		this->isStart = false;
		this->isInitialize = false;
		this->isBooting = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerIceMagic::Draw()const
{
	/*移動していなかったらreturnを返す*/
	if (!this->isStart)return;
	VECTOR endPosition = this->position;
	endPosition.y -= this->radius;
	DrawCapsule3D(this->position, endPosition,this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}