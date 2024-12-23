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
	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		_player.DeathProcess();
		this->isEndAction = true;
	}
}