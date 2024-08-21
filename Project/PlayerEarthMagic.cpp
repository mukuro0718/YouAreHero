#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "PlayerMagic.h"
#include "PlayerEarthMagic.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerEarthMagic::PlayerEarthMagic()
	: frameCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerEarthMagic::~PlayerEarthMagic()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerEarthMagic::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*定数型の初期値を作成*/
	const float RADIUS = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_RADIUS"];
	/*変数を初期化*/
	this->position = player.GetPosition();
	this->radius = RADIUS;
	this->frameCount = 0;
	this->isInitialize = true;
}

/// <summary>
/// 更新
/// </summary>
void PlayerEarthMagic::Update()
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
	int MAX_FRAME_COUNT = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_INTERVAL"];
	this->frameCount++;

	/*フレームが最大値以上だったら各フラグを下す*/
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
const void PlayerEarthMagic::Draw()const
{
	/*移動していなかったらreturnを返す*/
	if (!this->isInitialize)return;

	DrawSphere3D(this->position, this->radius, 16, GetColor(100, 200, 100), GetColor(100, 200, 100), FALSE);
}