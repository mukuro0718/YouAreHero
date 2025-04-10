#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_PlayCurrentBattleAction.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_PlayCurrentBattleAction::Beast_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_PlayCurrentBattleAction::~Beast_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Beast_PlayCurrentBattleAction::NodeState Beast_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	return _tree.GetBattleAction().Update(_tree, _chara);
}
