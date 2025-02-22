#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

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
Condition_IsNowStateIsSameAsTheSpecifiedState::NodeState Condition_IsNowStateIsSameAsTheSpecifiedState::Update()
{
	auto& beast = Singleton<EnemyManager>::GetInstance();

	/*��Ԃ��w��̏�ԂłȂ����FAILURE��Ԃ�*/
	if (beast.GetBossState() != this->SPECIFIED_STATE)
	{
		//printfDx("CONDITION_STATE_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_STATE_SUCCESS\n");
	return NodeState::SUCCESS;
}