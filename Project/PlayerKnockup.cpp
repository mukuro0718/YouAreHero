#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerKnockup.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerKnockup::PlayerKnockup()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::DOWN_UP);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->canselFrame			= json.GetJson(JsonManager::FileType::PLAYER)["KNOCK_UP_CANCEL_FRAME"];

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerKnockup::~PlayerKnockup()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerKnockup::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerKnockup::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerKnockup::Update(Player& _player)
{
	/*スタミナの回復*/
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*無敵フラグを下す*/
	if (this->frameCount == 0)
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*移動速度が０以上の時処理を行う*/
	if (_player.GetSpeed() != 0 || this->frameCount == 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), 0.0f, true, false);
		Move(_player, data);
	}

	this->frameCount++;
	if (this->canselFrame == this->frameCount)
	{
		this->isChangeAction = true;
	}

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*アニメーションの再生が終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}