#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerBoltMagic.h"
#include "CameraManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerBoltMagic::PlayerBoltMagic()
	: endPosition{ 0.0f,0.0f,0.0f }
	, velocity(0.0f)
	, frameCount()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerBoltMagic::~PlayerBoltMagic()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerBoltMagic::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*定数型の初期値を作成*/
	const float POSITION_OFFSET	  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_POSITON_OFFSET"];
	const float POSITION_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_POSITON_Y_OFFSET"];
	const float RADIUS			  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RADIUS"];
	const float RANGE_OFFSET_Y	  = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RANGE_BEFORE_OFFSET"];
	/*変数を初期化*/
	this->moveVector	=  VScale(camera.GetCameraDirection(), -1.0f);
	this->moveVector	=  VNorm(this->moveVector);
	this->position		=  player.GetPosition() + (this->moveVector * POSITION_OFFSET);
	this->position.y	+= POSITION_OFFSET_Y;
	this->endPosition	=  this->position;
	this->endPosition.y -= RANGE_OFFSET_Y;
	this->radius		=  RADIUS;
	this->frameCount	=  0;
	this->isInitialize	=  true;
	this->isBooting		= false;
}

/// <summary>
/// 更新
/// </summary>
void PlayerBoltMagic::Update()
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
	int BEFORE_MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_BEFORE_INTERVAL"];
	int AFTER_MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_AFTER_INTERVAL"];
	this->frameCount++;

	/*魔法が起動していないときフレームが最大値以上だったら魔法を起動する*/
	if (!this->isBooting && this->frameCount >= BEFORE_MAX_FRAME_COUNT)
	{
		this->isBooting = true;
		this->frameCount = 0;
	}

	/*魔法が起動していなかったら早期リターン*/
	if (!this->isBooting)return;

	/*当たり判定用のカプセルの終点を伸ばす*/
	const float RANGE_OFFSET_Y = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_RANGE_AFTER_OFFSET"];
	this->endPosition = this->position;
	this->endPosition.y -= RANGE_OFFSET_Y;
	
	if (this->frameCount >= AFTER_MAX_FRAME_COUNT)
	{
		this->isInitialize = false;
		this->isBooting = false;
		this->frameCount = 0;
		this->isStart = false;
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerBoltMagic::Draw()const
{
	/*移動していなかったらreturnを返す*/
	if (!this->isInitialize)return;

	DrawCapsule3D(this->position, this->endPosition, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}