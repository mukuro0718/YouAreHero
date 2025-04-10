#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_PlayCurrentBattleAction.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Tank_PlayCurrentBattleAction::Tank_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Tank_PlayCurrentBattleAction::~Tank_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Tank_PlayCurrentBattleAction::NodeState Tank_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	return _tree.GetBattleAction().Update(_tree, _chara);
}
