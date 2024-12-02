#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerWalk::~PlayerWalk()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerWalk::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerWalk::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerWalk::Update(Player& _player)
{
	this->isEndAction = true;

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"], false, false);
	Move(_player, data);

	/*スタミナの回復*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::WALK_FRONT);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}