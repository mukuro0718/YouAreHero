#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_PlayCurrentReaction.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Tank_PlayCurrentReaction::Tank_PlayCurrentReaction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Tank_PlayCurrentReaction::~Tank_PlayCurrentReaction()
{

}


/// <summary>
/// ������
/// </summary>
void Tank_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
Tank_PlayCurrentReaction::NodeState Tank_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
