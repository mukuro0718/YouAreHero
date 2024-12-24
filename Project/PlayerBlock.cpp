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
	this->staminaRecoveryValue = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->maxStamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	this->enemyFirstDirection = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
	this->playerFirstDirection = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
	this->toleranceDot = json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];
	this->staminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->nextAnimation = static_cast<int>(Player::AnimationType::BLOCK);
	this->playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
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
	/*スタミナの回復*/
	_player.CalcStamina(this->staminaRecoveryValue, this->maxStamina);

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	auto& input = Singleton<InputManager>  ::GetInstance();
	bool isSkip = true;
	float maxSpeed = 0.0f;
	if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		isSkip = false;
		maxSpeed = this->maxSpeed;
	}
	MoveData data;
	data.Set(_player.GetNextRotation(), maxSpeed, isSkip, false);
	Move(_player, data);

	/*ガードが成功しているかの判定をする*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
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