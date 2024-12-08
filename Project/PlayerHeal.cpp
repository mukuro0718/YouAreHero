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

/// <summary>
/// コンストラクタ
/// </summary>
PlayerHeal::PlayerHeal()
	: PlayerAction()
	, isPlay(false)
{

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
	this->isChangeAction = false;
	this->isEndAction = false;
	this->isPlay = false;
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
	auto& json = Singleton<JsonManager>::GetInstance();
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*開始時に行う処理*/
	if (!this->isPlay)
	{
		//HPを回復（最大を超えないようにする）
		int hp = _player.GetPlayerData().hp;
		hp += json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
		if (hp >= json.GetJson(JsonManager::FileType::PLAYER)["HP"])
		{
			hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
		}
		_player.GetPlayerData().hp = hp;
		//回復回数の減少
		int healCount = _player.GetHealCount();
		healCount--;
		_player.SetHealCount(healCount);
		//回復エフェクトの再生
		auto& effect = Singleton<EffectManager> ::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
		this->isPlay = true;
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::HEAL);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}