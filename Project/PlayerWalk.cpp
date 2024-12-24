#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{
	auto& json					= Singleton<JsonManager>::GetInstance();
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::WALK_FRONT);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
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
	this->isEndAction = true;

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, false);
	Move(_player, data);

	/*スタミナの回復*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}