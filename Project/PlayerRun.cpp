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

/// <summary>
/// コンストラクタ
/// </summary>
PlayerRun::PlayerRun()
	: PlayerAction()
{

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
	this->isEndAction = true;

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"], false, true);
	Move(_player, data);

	/*スタミナの計算*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);
	
	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}