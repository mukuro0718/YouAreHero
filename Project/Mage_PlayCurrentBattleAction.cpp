#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_PlayCurrentBattleAction.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Mage_PlayCurrentBattleAction::Mage_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Mage_PlayCurrentBattleAction::~Mage_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Mage_PlayCurrentBattleAction::NodeState Mage_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	return _tree.GetBattleAction().Update(_tree, _chara);
}
