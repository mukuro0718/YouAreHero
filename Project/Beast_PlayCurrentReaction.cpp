#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_PlayCurrentReaction.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Beast_PlayCurrentReaction::Beast_PlayCurrentReaction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Beast_PlayCurrentReaction::~Beast_PlayCurrentReaction()
{

}


/// <summary>
/// ������
/// </summary>
void Beast_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
Beast_PlayCurrentReaction::NodeState Beast_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
