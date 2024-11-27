#include <DxLib.h>
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerIdle.h"
#include "UseJson.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerIdle::PlayerIdle()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerIdle::~PlayerIdle()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerIdle::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void PlayerIdle::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerIdle::Update(Player& _player)
{
	/*アニメーションの再生*/
	auto& json = Singleton<JsonManager>::GetInstance();
	int nextAnimation = static_cast<int>(Player::AnimationType::IDLE);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}