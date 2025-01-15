#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ReactionType.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerKnockdown.h"
#include "EffectManager.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerKnockdown::PlayerKnockdown()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][static_cast<int>(Gori::PlayerReactionType::BLOW_BIG)];
	this->nextAnimation = static_cast<int>(Player::AnimationType::DOWN_REACTION);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerKnockdown::~PlayerKnockdown()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerKnockdown::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerKnockdown::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerKnockdown::Update(Player& _player)
{
	/*スタミナの回復*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

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

	/*処理の開始時に一度だけ行う処理*/
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//スピードの設定
		const CharacterData& data = _player.GetCharacterData();
		_player.SetSpeed(this->maxSpeed);
		//ヒットストップの設定
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
		//無敵フラグを立てる
		_player.GetPlayerData().isInvinvible = true;
	}

	/*アニメーションの再生*/
	bool isStopAnimation = false;
	if (this->frameCount != 0 && _player.GetIsChangeAnimation())
	{
		isStopAnimation = true;
	}
	if (!isStopAnimation)
	{
		_player.PlayAnimation(this->nextAnimation, this->playTime);
	}

	/*アニメーションの再生が終了していたら早期リターン*/
	auto& input = Singleton<InputManager>  ::GetInstance();
	bool isChangeAnim = _player.GetIsChangeAnimation();
	if (isChangeAnim && (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0))
	{
		this->isEndAction = true;
	}

	/*フレーム計測*/
	this->frameCount++;
}