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
{
	auto& json		= Singleton<JsonManager>::GetInstance();
	this->maxStamina			= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	this->staminaRecoveryValue	= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"];
	this->firstSpeed			= json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	this->staminaConsumpion		= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"];
	this->rotatableFrame		= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"];
	this->justAvoidFrame		= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"];
	this->nextAnimation			= static_cast<int>(Player::AnimationType::AVOID);
	this->playTime				= json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nextAnimation];
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
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
	this->isRotate = false;

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
	if (this->frameCount == 0)
	{
		_player.GetPlayerData().isInvinvible = true;
		_player.CalcStamina(this->staminaConsumpion, this->maxStamina);
		_player.SetSpeed(this->firstSpeed);
	}
	else
	{
		//アニメーションが終了しているか
		if (_player.GetIsChangeAnimation())
		{
			this->isEndAction = true;
			this->isRotate = false;
		}
	}

	/*フレームの計測*/
	if (!this->isRotate)
	{
		//フレームカウントの増加
		this->frameCount++;
		//アクションキャンセルが可能だったら
		if (!this->isRotate && (this->frameCount >= this->rotatableFrame))
		{
			this->isChangeAction = true;
			this->isRotate = true;
		}
		//無敵時間
		if (this->frameCount >= this->justAvoidFrame)
		{
			_player.GetPlayerData().isInvinvible = false;
		}
	}

	/*移動処理*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, this->isRotate, false);
	Move(_player, data);

	/*アニメーションの再生*/
	_player.PlayAnimation(this->nextAnimation, this->playTime);
}