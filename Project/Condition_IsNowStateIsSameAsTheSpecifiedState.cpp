#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsNowStateIsSameAsTheSpecifiedState::Condition_IsNowStateIsSameAsTheSpecifiedState(const int _specifiedState)
	: SPECIFIED_STATE(_specifiedState)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsNowStateIsSameAsTheSpecifiedState::~Condition_IsNowStateIsSameAsTheSpecifiedState()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsNowStateIsSameAsTheSpecifiedState::NodeState Condition_IsNowStateIsSameAsTheSpecifiedState::Update(BehaviorTree& _tree, Character& _chara)
{
	auto& boss = dynamic_cast<Enemy&>(_chara);

	/*��Ԃ��w��̏�ԂłȂ����FAILURE��Ԃ�*/
	if (boss.GetBossState() != this->SPECIFIED_STATE)
	{
		//printfDx("CONDITION_STATE_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_STATE_SUCCESS\n");
	return NodeState::SUCCESS;
}