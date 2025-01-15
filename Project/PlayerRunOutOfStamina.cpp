#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRunOutOfStamina.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerRunOutOfStamina::PlayerRunOutOfStamina()
	: PlayerAction()
{
	auto& json	 = Singleton<JsonManager>::GetInstance();
	this->maxSpeed			 = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
	this->maxStamina		 = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->nextAnimation		 = static_cast<int>(Player::AnimationType::RUN_FRONT);
	this->playTime			 = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerRunOutOfStamina::~PlayerRunOutOfStamina()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerRunOutOfStamina::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerRunOutOfStamina::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerRunOutOfStamina::Update(Player& _player)
{
	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(Gori::ORIGIN, this->maxSpeed, false, true);
	Move(_player, data);

	/*スタミナの計算*/
	_player.CalcStamina(this->staminaConsumption, this->maxStamina);
	
	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	this->isEndAction = true;
}