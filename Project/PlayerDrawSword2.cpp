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
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation = static_cast<int>(Player::AnimationType::DRAW_SWORD_2);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->cancelableFrame = json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_2_CANCEL_FRAME"];
	this->frameIndex = json.GetJson(JsonManager::FileType::PLAYER)["DRAW_SWORD_VISIBLE_FRAME"];
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
	/*回転の更新*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	VECTOR nextRotation = _player.GetNextRotation();
	UpdateRotation(true, nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*移動速度の更新*/
	_player.SetSpeed(0.0f);

	/*移動ベクトルを出す*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	/*最初に武器を表示する*/
	if (this->frameCount == 0)
	{
		MV1SetFrameVisible(_player.GetModelHandle(), this->frameIndex, true);
		_player.SetIsDrawSword(true);
	}

	/*キャンセルフラグが立っていなったら*/
	if (!this->isChangeAction)
	{
		this->frameCount++;
		if (this->frameCount >= this->cancelableFrame)
		{
			this->isChangeAction = true;
		}
	}

	/*アニメーションが終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}