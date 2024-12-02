#include <DxLib.h>
#include "UseSTL.h"
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
	: PlayerAction	()
	, isPlay		(false)
	, frameCount	(0)
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
	this->isEndAction = false;
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
	else
	{
		//アニメーションが終了しているか
		if (_player.GetIsChangeAnimation())
		{
			this->isEndAction = true;
		}
	}

	/*フレームの計測*/
	bool isRotate = true;
	//フレームカウントの増加
	this->frameCount++;
	//アクションキャンセルが可能だったら
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"])
	{
		isRotate = false;
	}
	//無敵時間
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"])
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, isRotate, false);
	Move(_player, data);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::AVOID);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}