#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_PlayCurrentReaction.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Mage_PlayCurrentReaction::Mage_PlayCurrentReaction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Mage_PlayCurrentReaction::~Mage_PlayCurrentReaction()
{

}


/// <summary>
/// ������
/// </summary>
void Mage_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
Mage_PlayCurrentReaction::NodeState Mage_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
