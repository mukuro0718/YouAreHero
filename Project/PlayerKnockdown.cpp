#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
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
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);


	/*処理の開始時に一度だけ行う処理*/
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//スピードの設定
		const CharacterData& data = _player.GetCharacterData();
		_player.SetSpeed(json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][data.reactionType]);
		//ヒットストップの設定
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
		//無敵フラグを立てる
		_player.GetPlayerData().isInvinvible = true;
		this->nextAnimation = static_cast<int>(Player::AnimationType::DOWN_REACTION);
	}

	/*アニメーションの再生*/
	bool isStopAnimation = false;
	if (this->frameCount != 0 && _player.GetIsChangeAnimation())
	{
		isStopAnimation = true;
	}
	if (!isStopAnimation)
	{
		float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
		_player.PlayAnimation(nextAnimation, playTime);
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