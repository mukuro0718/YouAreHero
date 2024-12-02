#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerStrongAttack.h"
#include "PlayerAttackManager.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStrongAttack::PlayerStrongAttack()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStrongAttack::~PlayerStrongAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerStrongAttack::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->isPlay = false;
	this->isCharge = true;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerStrongAttack::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerStrongAttack::Update(Player& _player)
{
	/*アニメーションが終了していたら早期リターン*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
		return;
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*溜め判定*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->isCharge)
	{
		auto& input = Singleton<InputManager> ::GetInstance();
		//攻撃ボタンを長押ししていたら
		if (input.GetNowPadState() & InputManager::PAD_Y)
		{
			this->frameCount++;
			//基底のフレームを超えたら攻撃を再開する
			if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_MAX_FRAME"])
			{
				this->isCharge = false;
			}
		}
		//攻撃ボタンを押していなかったら、攻撃を再開する
		else
		{
			this->isCharge = false;
		}
	}
	//溜めていなければ
	else
	{
		if (!this->isPlay)
		{
			auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
			//ダメージの計算
			int damageFactor = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE_FACTOR"];
			int baseDamage = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE"];
			int damage = baseDamage * (this->frameCount / damageFactor);
			//ベースよりも小さくならないようにする
			if (damage < baseDamage)
			{
				damage = baseDamage;
			}
			//ダメージのセット
			attack.SetDamage(damage);
			//攻撃の開始
			attack.OnIsStart();
			this->isPlay = true;
		}
	}

	/*アニメーションの更新*/
	//溜め攻撃中に攻撃アニメーション停止フレームを超えていたらアニメーションを停止する
	bool isStopAnimation = false;
	if (this->isCharge && this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_FRAME"])
	{
		isStopAnimation = true;
	}
	//停止フラグが立っていなければ再生する
	if (!isStopAnimation)
	{
		int nextAnimation = static_cast<int>(Player::AnimationType::SKILL);
		int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
		_player.PlayAnimation(nextAnimation, playTime);
	}

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isChangeAction = true;
	}
}