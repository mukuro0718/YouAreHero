#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "CharacterData.h"
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
	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*処理の開始時に一度だけ行う処理*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>  ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);
		//スピードの設定
		const CharacterData& data = _player.GetCharacterData();
		_player.SetSpeed(json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][data.playerReaction]);
		//ヒットストップの設定
		_player.SetHitStop(data.hitStopTime, data.hitStopType, data.hitStopDelay, data.slowFactor);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
	}

	/*フレーム計測*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::REACTION);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*アニメーションの再生が終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}