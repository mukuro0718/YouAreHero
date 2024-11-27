#include <DxLib.h>
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerAvoid.h"
#include "UseJson.h"
#include "CharacterData.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAvoid::PlayerAvoid()
	: PlayerAction()
	, isPlay (false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAvoid::~PlayerAvoid()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerAvoid::Initialize()
{
	this->isPlay = false;
	this->isChangeAction = false;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerAvoid::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void PlayerAvoid::Update(Player& _player)
{
	/*開始直後の処理*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->frameCount == 0)
	{
		//無敵フラグを立てる
		_player.GetPlayerData().isInvinvible = true;
		//スタミナの計算
		_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		//初速を入れる
		_player.SetSpeed(json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"]);
	}

	/*フレームの計測*/
	//フレームカウントの増加
	this->frameCount++;
	//アニメーションが終了しているか回避キャンセルが可能だったら
	if (_player.GetIsChangeAnimation() || this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"])
	{
		this->isChangeAction = true;
	}
	//無敵時間
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"])
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*回転率を出す*/
	VECTOR nextRotation = _player.GetNextRotation();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*移動速度を出す*/
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*移動ベクトルを出す*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}