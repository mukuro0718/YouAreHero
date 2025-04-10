#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsSelectedReaction::Condition_IsSelectedReaction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsSelectedReaction::~Condition_IsSelectedReaction()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsSelectedReaction::NodeState Condition_IsSelectedReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	/*���ʔj�󂳂�Ă����琬��*/
	if (_tree.GetIsSelectedReaction())
	{
		//printfDx("CONDITION_SELECTED_REACTION_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_REACTION_FAILURE\n");
	return NodeState::FAILURE;
}