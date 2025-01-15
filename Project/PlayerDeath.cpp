#include <DxLib.h>
#include "UseSTL.h"
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
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation = static_cast<int>(Player::AnimationType::DEATH);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

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
	this->isChangeAction = false;
	this->isEndAction = true;
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
	/*回転の更新*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	VECTOR nextRotation = _player.GetNextRotation();
	UpdateRotation(true, nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*移動速度の更新*/
	_player.SetSpeed(0.0f);

	/*移動ベクトルを出す*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		_player.DeathProcess();
		this->isEndAction = true;
	}
}