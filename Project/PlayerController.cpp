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

	/*キャンセルフラグが立っているときに行えるアクション群*/
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::AVOID));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_2));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::COMBO_3));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::STRONG_ATTACK));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::DRAW_SWORD_1));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::DRAW_SWORD_2));
	this->stateTheIsCancel.emplace_back(static_cast<int>(PlayerState::KNOCK_UP));

	this->blockStaminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->avoidStaminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"];
	this->runStaminaConsumption	  = json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
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
bool PlayerController::StateChanger(const bool _isCancelAction, const bool _isEndAction, const bool _isDrawSword, const CharacterData& _data)
{
	PlayerState nextState;

	//auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager>  ::GetInstance();
	int nowPadState = input.GetNowPadState();

	/*HPが0以下なら状態をDEATHにする*/
	if (_data.hp <= 0 )
	{
		nextState = PlayerState::DEATH;
	}

	/*攻撃がヒットしていたらリアクションの種類別の状態にする*/
	else if (_data.isHit && !_data.isInvinvible)
	{
		//ガードしていたら
		if (_data.isGuard)
		{
			nextState = PlayerState::BLOCK_STAGGER;
		}
		else
		{
			//リアクション
			switch (_data.reactionType)
			{
			case static_cast<int>(Gori::PlayerReactionType::NORMAL):
				nextState = PlayerState::STAGGER;
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL):
				nextState = PlayerState::STAGGER;
				break;
			case static_cast<int>(Gori::PlayerReactionType::BLOW_BIG):
				nextState = PlayerState::KNOCK_DOWN;
				break;
			}
		}
	}

	/*アクションが終了しておらず、キャンセルフラグもたっていなかったら早期リターン*/
	else if (!_isEndAction && !_isCancelAction)
	{
		return false;
	}

	/*起き上がる*/
	else if (this->nowState == static_cast<int>(PlayerState::KNOCK_DOWN))
	{
		nextState = PlayerState::KNOCK_UP;
	}
	/*ガード*/
	//スタミナは足りているか
	else if (CanAction(_data.stamina, this->blockStaminaConsumption) && nowPadState & InputManager::PAD_RT)
	{
		if (_isDrawSword)
		{
			nextState = PlayerState::BLOCK;
		}
		else
		{
			nextState = PlayerState::DRAW_SWORD_2;
		}
	}

	/*弱攻撃*/
	else if (nowPadState & InputManager::PAD_B)
	{
		if (_isDrawSword)
		{
			switch (this->nowState)
			{
			case static_cast<int>(PlayerState::COMBO_1):
				nextState = PlayerState::COMBO_2;
				break;
			case static_cast<int>(PlayerState::COMBO_2):
				nextState = PlayerState::COMBO_3;
				break;
			default:
				nextState = PlayerState::COMBO_1;
				break;
			}
		}
		else
		{
			nextState = PlayerState::DRAW_SWORD_2;
		}
	}

	/*強攻撃*/
	else if (nowPadState & InputManager::PAD_Y)
	{
		if (_isDrawSword)
		{
			nextState = PlayerState::STRONG_ATTACK;
		}
		else
		{
			nextState = PlayerState::DRAW_SWORD_2;
		}
	}

	/*回避*/
	else if (CanAction(_data.stamina, this->avoidStaminaConsumption) && nowPadState & InputManager::PAD_A)
	{
		nextState = PlayerState::AVOID;
	}

	/*回復*/
	else if (nowPadState & InputManager::PAD_X)
	{
		nextState = PlayerState::HEAL;
	}

	/*移動*/
	else if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		if (nowPadState & InputManager::PAD_RB)
		{
			if (!_isDrawSword)
			{
				//ダッシュ
				if (CanAction(_data.stamina, this->runStaminaConsumption))
				{
					nextState = PlayerState::RUN;
				}
				//スタミナ切れ
				else
				{
					nextState = PlayerState::RUNNING_OUT_OF_STAMINA;
				}
			}
			else
			{
				nextState = PlayerState::DRAW_SWORD_1;
			}
		}
		else
		{
			//歩き
			nextState = PlayerState::WALK;
		}
	}
	else
	{
		nextState = PlayerState::IDLE;
	}

	int newState = static_cast<int>(nextState);

	/*アクションが終了しているか、選択された状態の優先順位が高いか*/
	if (_isEndAction || this->priority[newState] > this->priority[this->nowState])
	{
		this->prevState = this->nowState;
		this->nowState = newState;
		return true;
	}

	/*キャンセルフラグが立っているかつキャンセルできるアクション*/
	else if (_isCancelAction)
	{
		//現在選択中のアクションと異なっていたら
		if (this->nowState != newState)
		{
			for (int state : this->stateTheIsCancel)
			{
				if (state == newState)
				{
					this->prevState = this->nowState;
					this->nowState = newState;
					return true;
				}
			}
		}
	}
	return false;
}

/// <summary>
/// 必要な消費スタミナと現在のスタミナの残量を比べてアクションが可能ならtrueを返す
/// </summary>
const bool PlayerController::CanAction(const float _stamina, const float _staminaConsumed)const
{
	/*スタミナの消費量が現在のスタミナの総量よりも多ければfalseを返す*/
	float staminaConsumed = _staminaConsumed * -1.0f;
	if (staminaConsumed > _stamina)return false;
	return true;
}
