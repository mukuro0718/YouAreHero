#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "CharacterData.h"
#include "PlayerBlock.h"
#include "UseJson.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerBlock::PlayerBlock()
	: PlayerAction()
{

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
	/*回転率を出す*/
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	/*移動速度が０以上だったら処理を行う*/
	if (nowSpeed > maxSpeed)
	{
		//移動速度を出す
		UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
		_player.SetSpeed(nowSpeed);
		//移動ベクトルを出す
		VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
		VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
		_player.SetVelocity(newVelocity);
	}

	/*ガードが成功しているかの判定をする*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR enemyFirstDirection	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);		 //敵が最初に向いている向き
	VECTOR playerFirstDirection = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);	 //プレイヤーが最初に向いている向き
	VECTOR enemyDirection		= VTransform(enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));	 //今のエネミーの向き
		   enemyDirection		= VNorm(enemyDirection);
	VECTOR playerDirection		= VTransform(playerFirstDirection, MGetRotY(_player.GetRigidbody().GetRotation().y));//今のプレイヤーの向き
		   playerDirection		= VNorm(playerDirection);
	float  TOLERANCE_DOT		= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];//内積の許容範囲
	float  enemyAndPlayerDot	= VDot(enemyDirection, playerDirection);
		   enemyAndPlayerDot	= enemyAndPlayerDot * 180.0f / DX_PI_F;
	//もし内積がマイナスなら＋に戻して判定する
	if (enemyAndPlayerDot < 0) { enemyAndPlayerDot *= -1.0f; }
	//内積が許容範囲かつスタミナが足りていたらガード成功
	if (enemyAndPlayerDot >= TOLERANCE_DOT && _player.CanAction(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]))
	{
		_player.GetPlayerData().isGuard = true;
	}
	else
	{
		_player.GetPlayerData().isGuard = false;
	}

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::BLOCK);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}