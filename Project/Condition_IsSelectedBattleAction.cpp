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
Condition_IsSelectedBattleAction::Condition_IsSelectedBattleAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsSelectedBattleAction::~Condition_IsSelectedBattleAction()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsSelectedBattleAction::NodeState Condition_IsSelectedBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�U���A�N�V�������I������Ă����琬��*/
	if (_tree.GetIsSelectedBattleAction())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}