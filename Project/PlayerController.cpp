#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ReactionType.h"
#include "CharacterData.h"
#include "PlayerController.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerController::PlayerController()
	: nowState(0)
	, prevState(0)
{
	/*優先フラグを付ける*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	vector<int> priority = json.GetJson(JsonManager::FileType::PLAYER)["ACTION_PRIORITY"];
	this->priority = priority;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerController::~PlayerController()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerController::Initialize()
{
	this->nowState = 0;
	this->prevState = 0;
}

/// <summary>
/// 状態変更クラス
/// </summary>
void PlayerController::StateChanger(const bool _isEndAction, const CharacterData& _data)
{
	int nextState = -1;

	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager>  ::GetInstance();

	/*HPが0以下なら状態をDEATHにする*/
	if (_data.hp <= 0 )
	{
		nextState = static_cast<int>(PlayerState::DEATH);
	}

	/*攻撃がヒットしていたらリアクションの種類別の状態にする*/
	else if (_data.isHit)
	{
		//ガードしていたら
		if (_data.isGuard)
		{
			nextState = static_cast<int>(PlayerState::BLOCK_STAGGER);
		}
		else
		{
			//リアクション
			switch (_data.playerReaction)
			{
			case static_cast<int>(Gori::PlayerReactionType::NORMAL):
				nextState = static_cast<int>(PlayerState::STAGGER);
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL):
				nextState = static_cast<int>(PlayerState::STAGGER);
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_BIG):
				nextState = static_cast<int>(PlayerState::KNOCK_DOWN);
				break;
			}
		}
	}

	/*ガード*/
	//スタミナは足りているか
	else if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]) && input.GetNowPadState() & InputManager::PAD_RT)
	{
		nextState = static_cast<int>(PlayerState::BLOCK);
	}

	/*弱攻撃*/
	else if (input.GetNowPadState() & InputManager::PAD_B)
	{
		switch (this->prevState)
		{
		case static_cast<int>(PlayerState::COMBO_1):
			nextState = static_cast<int>(PlayerState::COMBO_2);
			break;
		case static_cast<int>(PlayerState::COMBO_2):
			nextState = static_cast<int>(PlayerState::COMBO_3);
			break;
		default:
			nextState = static_cast<int>(PlayerState::COMBO_1);
			break;
		}
	}

	/*強攻撃*/
	else if (input.GetNowPadState() & InputManager::PAD_Y)
	{
		nextState = static_cast<int>(PlayerState::STRONG_ATTACK);
	}

	/*回避*/
	else if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]) && input.GetNowPadState() & InputManager::PAD_A)
	{
		nextState = static_cast<int>(PlayerState::AVOID);
	}

	/*回復*/
	else if (input.GetNowPadState() & InputManager::PAD_X)
	{
		nextState = static_cast<int>(PlayerState::HEAL);
	}

	/*移動*/
	else if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		if (input.GetNowPadState() & InputManager::PAD_RB)
		{
			//ダッシュ
			if (CanAction(_data.stamina, json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
			{
				nextState = static_cast<int>(PlayerState::RUN);
			}
			//スタミナ切れ
			else
			{
				nextState = static_cast<int>(PlayerState::RUNNING_OUT_OF_STAMINA);
			}
		}
		else
		{
			//歩き
			nextState = static_cast<int>(PlayerState::WALK);
		}
	}
	else
	{
		nextState = static_cast<int>(PlayerState::IDLE);
	}

	/*アクションが終了しているか、選択された状態の優先順位が高いか*/
	if (_isEndAction || this->priority[nextState] > this->priority[this->nowState])
	{
		this->prevState = this->nowState;
		this->nowState = nextState;
	}
}

/// <summary>
/// 必要な消費スタミナと現在のスタミナの残量を比べてアクションが可能ならtrueを返す
/// </summary>
const bool PlayerController::CanAction(const float _stamina, const float _staminaConsumed)const
{
	/*スタミナの消費量が現在のスタミナの総量よりも多ければfalseを返す*/
	if (_staminaConsumed > _stamina)return false;
	return true;
}
