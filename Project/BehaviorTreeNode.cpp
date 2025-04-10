#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
BehaviorTreeNode::BehaviorTreeNode()
	: state(NodeState::NONE_ACTIVE)
{

}