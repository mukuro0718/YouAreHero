#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerBlockStagger.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerBlockStagger::PlayerBlockStagger()
	: PlayerAction()
	, MAX_SPEED(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAGGER_SPEED"])
	, DECEL(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::PLAYER)["ACCEL"])
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->maxStamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::BLOCK_REACTION);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerBlockStagger::~PlayerBlockStagger()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerBlockStagger::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
	this->nowSpeed = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerBlockStagger::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerBlockStagger::Update(Player& _player)
{
	/*移動速度の更新*/
	if (this->nowSpeed != 0)
	{
		this->nowSpeed += this->DECEL;
		if (this->nowSpeed > 0.0f)
		{
			this->nowSpeed = 0.0f;
		}
		_player.SetSpeed(this->nowSpeed);
	}

	if (_player.GetSpeed() != 0)
	{
		MoveData data;
		data.Set(_player.GetNextRotation(), this->nowSpeed, true, false);
		Move(_player, data);
	}

	/*処理の開始時に一度だけ行う処理*/
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
		//スタミナを減らす
		_player.CalcStamina(this->staminaConsumption, this->maxStamina);
		this->nowSpeed = this->MAX_SPEED;
		this->frameCount++;
	}

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}