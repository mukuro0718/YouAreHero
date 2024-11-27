#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRun.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerRun::PlayerRun()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerRun::~PlayerRun()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerRun::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerRun::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerRun::Update(Player& _player)
{
	/*回転率を出す*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*最大速度を出す*/
	auto& json = Singleton<JsonManager>::GetInstance();
	bool isRun = false;
	float maxSpeed = 0.0f;
	//スタミナが足りている
	if (_player.CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
	}
	//足りていない
	else
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	}

	/*スタミナの計算*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);

	/*移動速度を出す*/
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);


	/*移動ベクトルを出す*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);
	
	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}