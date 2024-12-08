#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerCombo3.h"
#include "PlayerAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerCombo3::PlayerCombo3()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerCombo3::~PlayerCombo3()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerCombo3::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->isPlay = false;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerCombo3::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerCombo3::Update(Player& _player)
{
	/*開始時に一度だけ呼ばれる*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (!this->isPlay)
	{
		auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
		attack.OnIsStart();
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][2]);
		this->isPlay = true;
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::COMBO_3);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}