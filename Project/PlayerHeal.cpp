#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerHeal.h"
#include "UseJson.h"
#include "Debug.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerHeal::PlayerHeal()
	: PlayerAction		()
	, HEAL_VALUE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"])
	, MAX_HP			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["HP"])
	, DO_HEAL_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["DO_HEAL_PLAY_TIME"])
	, MAX_HEAL_VALUE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_VALUE"])
	, nowTotalHealValue	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::HEAL);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerHeal::~PlayerHeal()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerHeal::Initialize()
{
	this->isChangeAction	= false;
	this->isEndAction		= false;
	this->frameCount		= 0;
	this->nowTotalHealValue = 0;
	this->nowTotalPlayTime  = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerHeal::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerHeal::Update(Player& _player)
{
	/*スタミナの回復*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*開始時に行う処理*/
	if (this->frameCount == 0)
	{
		//サウンドエフェクトの再生
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_HEAL);
		//回復回数の減少
		int healCount = _player.GetHealCount();
		healCount--;
		_player.SetHealCount(healCount);
		//回復エフェクトの再生
		auto& effect = Singleton<EffectManager> ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
		this->frameCount++;
	}

	/*HPを回復*/
	if (this->nowTotalPlayTime > this->DO_HEAL_PLAY_TIME)
	{
		if (this->nowTotalHealValue < this->MAX_HEAL_VALUE)
		{
			//HPを回復（最大を超えないようにする）
			int hp = _player.GetPlayerData().hp;
			hp += this->HEAL_VALUE;
			this->nowTotalHealValue += this->HEAL_VALUE;
			if (hp >= this->MAX_HP)
			{
				hp = this->MAX_HP;
			}
			_player.GetPlayerData().hp = hp;
		}
	}

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
	this->nowTotalPlayTime += this->playTime;
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}