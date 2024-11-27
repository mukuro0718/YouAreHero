#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerWalk::~PlayerWalk()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerWalk::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerWalk::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerWalk::Update(Player& _player)
{
	/*回転率を出す*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*移動速度を出す*/
	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*移動ベクトルを出す*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::WALK_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}