#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRun.h"
#include "UseJson.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerRun::PlayerRun()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::RUN_FRONT);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerRun::~PlayerRun()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerRun::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerRun::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerRun::Update(Player& _player)
{
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (!sound.GetCheckEffectSoundState(SoundManager::EffectType::PLAYER_RUN))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::PLAYER_RUN);
	}

	this->isEndAction = true;

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, true);
	Move(_player, data);

	/*スタミナの計算*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);
	
	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}