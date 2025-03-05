#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ReactionType.h"
#include "CharacterData.h"
#include "PlayerController.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerController::PlayerController()
	: nowState(0)
	, prevState(0)
{
	/*�D��t���O��t����*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	vector<short> priority = json.GetJson(JsonManager::FileType::PLAYER)["ACTION_PRIORITY"];
	this->priority = priority;

	/*�L�����Z���t���O�������Ă���Ƃ��ɍs����A�N�V�����Q*/
	//�R���{�P
	list<short> connectedActionsOfCombo1;
	connectedActionsOfCombo1.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo1.emplace_back(static_cast<int>(PlayerState::COMBO_2));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_1), connectedActionsOfCombo1);
	//�R���{�Q
	list<short> connectedActionsOfCombo2;
	connectedActionsOfCombo2.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo2.emplace_back(static_cast<int>(PlayerState::COMBO_3));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_2), connectedActionsOfCombo2);
	//�R���{3
	list<short> connectedActionsOfCombo3;
	connectedActionsOfCombo3.emplace_back(static_cast<int>(PlayerState::AVOID));
	connectedActionsOfCombo3.emplace_back(static_cast<int>(PlayerState::STRONG_ATTACK));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::COMBO_3), connectedActionsOfCombo3);
	//���U��
	list<short> connectedActionsOfStrongAttack;
	connectedActionsOfStrongAttack.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::STRONG_ATTACK), connectedActionsOfStrongAttack);
	//�[��
	list<short> connectedActionsOfDrawSword1;
	connectedActionsOfDrawSword1.emplace_back(static_cast<int>(PlayerState::RUN));
	connectedActionsOfDrawSword1.emplace_back(static_cast<int>(PlayerState::AVOID));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::DRAW_SWORD_1), connectedActionsOfDrawSword1);
	//����
	list<short> connectedActionsOfDrawSword2;
	connectedActionsOfDrawSword2.emplace_back(static_cast<int>(PlayerState::COMBO_1));
	this->stateTheIsCancel.emplace(static_cast<int>(PlayerState::DRAW_SWORD_2), connectedActionsOfDrawSword2);
	//�m�b�N�_�E��
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
/// �f�X�g���N�^
/// </summary>
PlayerController::~PlayerController()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerController::Initialize()
{
	this->nowState = 0;
	this->prevState = 0;
}

/// <summary>
/// ��ԕύX�N���X
/// </summary>
bool PlayerController::StateChanger(const bool _isCancelAction, const bool _isEndAction, const bool _isDrawSword, const CharacterData& _data, const int _healCount)
{
	PlayerState nextState = PlayerState::IDLE;

	//auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager>  ::GetInstance();

	/*HP��0�ȉ��Ȃ��Ԃ�DEATH�ɂ���*/
	if (_data.hp <= 0 )
	{
		nextState = PlayerState::DEATH;
	}

	/*�U�����q�b�g���Ă����烊�A�N�V�����̎�ޕʂ̏�Ԃɂ���*/
	else if (_data.isHit && !_data.isInvinvible)
	{
		//�K�[�h���Ă�����
		if (_data.isGuard)
		{
			nextState = PlayerState::BLOCK_STAGGER;
		}
		else
		{
			//���A�N�V����
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

	/*�A�N�V�������I�����Ă��炸�A�L�����Z���t���O�������Ă��Ȃ������瑁�����^�[��*/
	else if (!_isEndAction && !_isCancelAction)
	{
		return false;
	}

	/*�N���オ��*/
	else if (this->nowState == static_cast<int>(PlayerState::KNOCK_DOWN))
	{
		nextState = PlayerState::KNOCK_UP;
	}
	/*�K�[�h*/
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

	/*��U��*/
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

	/*���U��*/
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

	/*���*/
	else if (CanAction(_data.stamina, this->avoidStaminaConsumption) && input.GetNowPad(InputManager::PAD_A))
	{
		nextState = PlayerState::AVOID;
	}

	/*��*/
	else if (input.GetNowPad(InputManager::PAD_X))
	{
		if (_healCount > 0)
		{
			nextState = PlayerState::HEAL;
		}
	}

	/*�ړ�*/
	else if (input.GetLStickState().XBuf != 0 || input.GetLStickState().YBuf != 0)
	{
		if (input.GetNowPad(InputManager::PAD_RB))
		{
			if (!_isDrawSword)
			{
				//�_�b�V��
				if (CanAction(_data.stamina, this->runStaminaConsumption))
				{
					nextState = PlayerState::RUN;
				}
				//�X�^�~�i�؂�
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
			//����
			nextState = PlayerState::WALK;
		}
	}
	else
	{
		nextState = PlayerState::IDLE;
	}

	int newState = static_cast<int>(nextState);

	/*�A�N�V�������I�����Ă��邩�A�I�����ꂽ��Ԃ̗D�揇�ʂ�������*/
	if (_isEndAction || this->priority[newState] > this->priority[this->nowState])
	{
		this->prevState = this->nowState;
		this->nowState = newState;
		return true;
	}

	/*�L�����Z���t���O�������Ă��邩�L�����Z���ł���A�N�V����*/
	else if (_isCancelAction)
	{
		//���ݑI�𒆂̃A�N�V���������������瑁�����^�[��
		if (this->nowState == newState) return false;
		//map���Ɏw��̏�Ԃ��L�[�Ƃ��ĕێ�����Ă�����
		auto it = this->stateTheIsCancel.find(this->nowState);
		if (it != this->stateTheIsCancel.end())
		{
			const std::list<short>& targetList = it->second;
			for (int action : targetList)
			{
				//�L�����Z���\�ȃA�N�V���������ׂ�
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
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
const bool PlayerController::CanAction(const float _stamina, const float _staminaConsumed)const
{
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	float staminaConsumed = _staminaConsumed * -1.0f;
	if (staminaConsumed > _stamina)return false;
	return true;
}
