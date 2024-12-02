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
{

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
	/*アニメーションの再生が終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
		return;
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*処理の開始時に一度だけ行う処理*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	if (this->frameCount == 0)
	{
		//エフェクトの再生
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
		//ヒットフラグを下す
		_player.GetPlayerData().isHit = false;
		//スタミナを減らす
		_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]);
	}


	/*フレーム計測*/
	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::BLOCK_REACTION);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}