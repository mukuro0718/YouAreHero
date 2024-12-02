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
	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"], false, true);
	Move(_player, data);

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
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);

	this->isEndAction = true;
}