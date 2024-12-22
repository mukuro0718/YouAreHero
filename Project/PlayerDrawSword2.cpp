#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerDrawSword2.h"
#include "PlayerAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerDrawSword2::PlayerDrawSword2()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerDrawSword2::~PlayerDrawSword2()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerDrawSword2::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerDrawSword2::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerDrawSword2::Update(Player& _player)
{

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*アニメーションの再生*/
	auto& json = Singleton<JsonManager>::GetInstance();
	int nextAnimation = static_cast<int>(Player::AnimationType::DRAW_SWORD_2);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	if (this->frameCount == 0)
	{
		MV1SetFrameVisible(_player.GetModelHandle(), json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_VISIBLE_FRAME"], true);
		_player.SetIsDrawSword(true);
	}

	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_2_CANCEL_FRAME"])
	{
		this->isChangeAction = true;
	}
	/*アニメーションが終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}