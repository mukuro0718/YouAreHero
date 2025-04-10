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
Condition_IsDestoroyedPart::Condition_IsDestoroyedPart()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDestoroyedPart::~Condition_IsDestoroyedPart()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDestoroyedPart::NodeState Condition_IsDestoroyedPart::Update(BehaviorTree& _tree, Character& _chara)
{
	/*���ʔj�󂳂�Ă����琬��*/
	if (_tree.GetIsDestroyedPart())
	{
		//_tree.SetIsDestroyedPart(false);
		//printfDx("CONDITION_DESTROY_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DESTROY_FAILURE\n");
	return NodeState::FAILURE;
}