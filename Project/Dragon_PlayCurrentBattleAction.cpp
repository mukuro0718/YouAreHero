#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Dragon_PlayCurrentBattleAction.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon_PlayCurrentBattleAction::Dragon_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon_PlayCurrentBattleAction::~Dragon_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Dragon_PlayCurrentBattleAction::NodeState Dragon_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	return _tree.GetBattleAction().Update(_tree, _chara);
}
