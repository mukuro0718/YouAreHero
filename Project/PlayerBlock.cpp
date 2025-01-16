#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "CharacterData.h"
#include "PlayerBlock.h"
#include "EnemyManager.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerBlock::PlayerBlock()
	: PlayerAction()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->maxSpeed				= json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	this->enemyFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
	this->playerFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
	this->toleranceDot			= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];
	this->staminaConsumption	= json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::BLOCK);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerBlock::~PlayerBlock()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerBlock::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerBlock::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerBlock::Update(Player& _player)
{
	/*回転処理*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	if (this->frameCount < 10)
	{
		VECTOR nextRotation = Gori::ORIGIN;
		VECTOR enemyPosition = enemy.GetRigidbody().GetPosition();
		VECTOR positionToEnemy = VSub(_player.GetRigidbody().GetPosition(), enemyPosition);
		nextRotation.y = static_cast<float>(atan2(static_cast<double>(positionToEnemy.x), static_cast<double>(positionToEnemy.z)));
		nowRotation = Gori::LerpAngle(nowRotation, nextRotation, this->rotateLerpValue);
		_player.SetRotation(nowRotation, nextRotation);

	}

	/*移動速度の更新*/
	_player.SetSpeed(0.0f);

	/*移動ベクトルを出す*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, 0.0f, false);
	_player.SetVelocity(newVelocity);

	/*ガードが成功しているかの判定をする*/
	VECTOR enemyDirection = VTransform(this->enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));	 //今のエネミーの向き
	enemyDirection = VNorm(enemyDirection);
	VECTOR playerDirection = VTransform(this->playerFirstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));//今のプレイヤーの向き
	playerDirection = VNorm(playerDirection);
	float  enemyAndPlayerDot = VDot(enemyDirection, playerDirection);
	enemyAndPlayerDot = enemyAndPlayerDot * 180.0f / DX_PI_F;
	//もし内積がマイナスなら＋に戻して判定する
	if (enemyAndPlayerDot < 0) { enemyAndPlayerDot *= -1.0f; }
	//内積が許容範囲かつスタミナが足りていたらガード成功
	if (enemyAndPlayerDot >= this->toleranceDot && _player.CanAction(this->staminaConsumption))
	{
		_player.GetPlayerData().isGuard = true;
	}
	else
	{
		_player.GetPlayerData().isGuard = false;
	}

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);

	this->isEndAction = true;
}