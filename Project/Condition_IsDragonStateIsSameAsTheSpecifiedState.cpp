#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonStateIsSameAsTheSpecifiedState::Condition_IsDragonStateIsSameAsTheSpecifiedState(const int _specifiedState)
	: SPECIFIED_STATE(_specifiedState)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonStateIsSameAsTheSpecifiedState::~Condition_IsDragonStateIsSameAsTheSpecifiedState()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonStateIsSameAsTheSpecifiedState::NodeState Condition_IsDragonStateIsSameAsTheSpecifiedState::Update()
{
	/*���ʔj�󂳂�Ă����琬��*/
	auto& dragon = Singleton<EnemyManager>::GetInstance();
	if (dragon.GetBossState() == SPECIFIED_STATE)
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}