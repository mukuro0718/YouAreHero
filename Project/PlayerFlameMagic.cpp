#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerFlameMagic.h"
#include "PlayerManager.h"
#include "CameraManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerFlameMagic::PlayerFlameMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, frameCount(0)
	, range(0.0f)
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerFlameMagic::~PlayerFlameMagic()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerFlameMagic::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

	/*使用する変数を初期化*/
	const float POSITION_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_POSITON_OFFSET"];
	this->moveVector			= camera.GetCameraDirection();
	this->moveVector			= VNorm(this->moveVector);
	this->position				= player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->radius				= json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_RADIUS"];
	this->range					= json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_RANGE"];
	this->moveVector			= VScale(this->moveVector, -this->range);
	this->endPosition			= this->position + this->moveVector;
	this->isInitialize			= true;
	this->frameCount			= 0;
}

/// <summary>
/// 更新
/// </summary>
void PlayerFlameMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_INTERVAL"];
	this->frameCount++;
	
	/*フレームが最大値だったら各フラグを下す*/
	if (this->frameCount >= MAX_FRAME_COUNT)
	{
		this->isInitialize = false;
		this->isStart = false;
		this->frameCount = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerFlameMagic::Draw()const
{
	/*初期化していなかったらreturnを返す*/
	if (!this->isInitialize)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}