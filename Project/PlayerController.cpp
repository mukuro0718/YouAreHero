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
	vector<short> priority = json.GetJson(JsonManager::FileType::PLAYER)["ACTION_PRIORITY"];
	this->priority = priority;

	/*キャンセルフラグが立っているときに行えるアクション群*/
	//コンボ１
	list<short> connectedActionsOfCombo1;
	connectedActionsOfCombo1.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo1.emplace_back(static_cast<int>(PlayerState::COMBO_2));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_1), connectedActionsOfCombo1);
	//コンボ２
	list<short> connectedActionsOfCombo2;
	connectedActionsOfCombo2.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo2.emplace_back(static_cast<int>(PlayerState::COMBO_3));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_2), connectedActionsOfCombo2);
	//コンボ3
	list<short> connectedActionsOfCombo3;
	connectedActionsOfCombo3.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo3.emplace_back(static_cast<int>(PlayerState::STRONG_ATTACK));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_3), connectedActionsOfCombo3);
	//強攻撃
	list<short> connectedActionsOfStrongAttack;
	connectedActionsOfStrongAttack.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::STRONG_ATTACK), connectedActionsOfStrongAttack);
	//納刀
	list<short> connectedActionsOfDrawSword1;
	connectedActionsOfDrawSword1.emplace_back(static_cast<int>(PlayerState::RUN));
	connectedActionsOfDrawSword1.emplace_back(static_cast<int>(PlayerState::AVOID));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::DRAW_SWORD_1), connectedActionsOfDrawSword1);
	//抜刀
	list<short> connectedActionsOfDrawSword2;
	connectedActionsOfDrawSword2.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::DRAW_SWORD_2), connectedActionsOfDrawSword2);
	//ノックダウン
	list<short> connectedActionsOfKnockDown;
	connectedActionsOfKnockDown.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfKnockDown.emplace_back(static_cast<int>(PlayerState::RUN));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::KNOCK_UP), connectedActionsOfKnockDown);

	this->blockStaminaConsumption		 = json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"];
	this->avoidStaminaConsumption		 = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"];
	this->runStaminaConsumption			 = json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"];
	this->weakAttackStaminaConsumption1	 = json.GetJson(JsonManager::FileType::PLAYER)["COMBO1_STAMINA_CONSUMPTION"];
	this->weakAttackStaminaConsumption2 = json.GetJson(JsonManager::FileType::PLAYER)["COMBO2_STAMINA_CONSUMPTION"];
	this->weakAttackStaminaConsumption3	 = json.GetJson(JsonManager::FileType::PLAYER)["COMBO3_STAMINA_CONSUMPTION"];
	this->strongAttackStaminaConsumption = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_STAMINA_CONSUMPTION"];
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
bool PlayerController::StateChanger(const bool _isCancelAction, const bool _isEndAction, const bool _isDrawSword, const CharacterData& _data, const int _healCount)
{
	PlayerState nextState = PlayerState::IDLE;

	//auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager>  ::GetInstance();

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
	else if (input.GetLeftTrigger())
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
	else if ( input.GetNowPad(InputManager::PAD_B))
	{
		if (_isDrawSword)
		{
			switch (this->nowState)
			{
			case static_cast<int>(PlayerState::COMBO_1):
				if (CanAction(_data.stamina, this->weakAttackStaminaConsumption2))
				{
					nextState = PlayerState::COMBO_2;
				}
				break;
			case static_cast<int>(PlayerState::COMBO_2):
				if (CanAction(_data.stamina, this->weakAttackStaminaConsumption3))
				{
					nextState = PlayerState::COMBO_3;
				}
				break;
			default:
				if (CanAction(_data.stamina, this->weakAttackStaminaConsumption1))
				{
					nextState = PlayerState::COMBO_1;
				}
				break;
			}
		}
		else
		{
			nextState = PlayerState::DRAW_SWORD_2;
		}
	}

	/*強攻撃*/
	else if (input.GetNowPad(InputManager::PAD_Y))
	{
		if (_isDrawSword)
		{
			if (CanAction(_data.stamina, this->strongAttackStaminaConsumption))
			{
				nextState = PlayerState::STRONG_ATTACK;
			}
		}
		else
		{
			nextState = PlayerState::DRAW_SWORD_2;
		}
	}

	/*回避*/
	else if (CanAction(_data.stamina, this->avoidStaminaConsumption) && input.GetNowPad(InputManager::PAD_A))
	{
		nextState = PlayerState::AVOID;
	}

	/*回復*/
	else if (input.GetNowPad(InputManager::PAD_X))
	{
		if (_healCount > 0)
		{
			nextState = PlayerState::HEAL;
		}
	}

	/*移動*/
	else if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		if (input.GetNowPad(InputManager::PAD_RB))
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
		//現在選択中のアクション同じだったら早期リターン
		if (this->nowState == newState) return false;
		//map内に指定の状態がキーとして保持されていたら
		auto it = this->stateTheIsCancel.find(this->nowState);
		if (it != this->stateTheIsCancel.end())
		{
			const std::list<short>& targetList = it->second;
			for (int action : targetList)
			{
				//キャンセル可能なアクションか調べる
				if (action == newState)
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
