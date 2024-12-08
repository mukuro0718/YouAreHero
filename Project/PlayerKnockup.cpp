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
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	_player.GetPlayerData().isInvinvible = false;

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::DOWN_UP);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*アニメーションの再生が終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}