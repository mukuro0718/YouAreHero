#include <DxLib.h>
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
		if (state == NodeState::SUCCESS)
		{
			//printfDx("SELECTOR_SUCCESS\n");
			return NodeState::SUCCESS;
		}
		//状態が実行中なら実行中を返し終了
		if (state == NodeState::RUNNING)
		{
			//printfDx("SELECTOR_RUNNING\n");
			return NodeState::RUNNING;
		}
		//printfDx("SELECTOR_FAILURE\n");
	}
	return NodeState::FAILURE;
}