#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo2.h"
#include "PlayerAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerCombo2::PlayerCombo2()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerCombo2::~PlayerCombo2()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerCombo2::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerCombo2::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerCombo2::Update(Player& _player)
{
	/*開始時に一度だけ呼ばれる*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->frameCount == 0)
	{
		auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
		//ダメージのセット
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][1]);
		attack.OnIsStart();
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::COMBO_2);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	if (this->frameCount == 0)
	{
		playTime = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_START_ANIM_PLAY_TIME"];
	}
	_player.PlayAnimation(nextAnimation, playTime);

	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_CANCEL_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}