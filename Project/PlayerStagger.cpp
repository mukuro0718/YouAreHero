#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ReactionType.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerStagger.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStagger::PlayerStagger()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	this->maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL)];
	this->cancelableFrame = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::REACTION);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStagger::~PlayerStagger()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerStagger::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerStagger::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerStagger::Update(Player& _player)
{

	/*処理の開始時に一度だけ行う処理*/
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//スピードの設定
		const CharacterData& data = _player.GetCharacterData();
		//ヒットストップの設定
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
	}
	/*移動速度が０以上の時処理を行う*/
	if (_player.GetSpeed() != 0)
	{
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
	}

	/*フレーム計測*/
	if (!this->isChangeAction)
	{
		this->frameCount++;
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
		}
	}

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*アニメーションの再生が終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}