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
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"], false, true);
	Move(_player, data);

	/*スタミナの計算*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);
	
	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	float playTime = 0.3f;
	_player.PlayAnimation(nextAnimation, playTime);
}