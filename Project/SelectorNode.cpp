#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "SelectorNode.h"

SelectorNode::SelectorNode()
{

}


SelectorNode::~SelectorNode()
{

}

/// <summary>
/// 更新
/// </summary>
SelectorNode::NodeState SelectorNode::Update()
{
	/*子を実行*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		//状態が成功なら成功を返し終了
		if (state == NodeState::SUCCESS) return NodeState::SUCCESS;
		//状態が実行中なら実行中を返し終了
		if (state == NodeState::RUNNING) return NodeState::RUNNING;
	}
	return NodeState::FAILURE;
}