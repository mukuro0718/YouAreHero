#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDeath.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerDeath::PlayerDeath()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerDeath::~PlayerDeath()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerDeath::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerDeath::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerDeath::Update(Player& _player)
{
	if (!_player.GetIsAlive()) return;

	/*移動速度を出す*/
	VECTOR nextRotation = _player.GetRigidbody().GetRotation();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	auto& json = Singleton<JsonManager>::GetInstance();
	bool isRun = false;
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*移動ベクトルを出す*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::DEATH);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		_player.DeathProcess();
	}
}