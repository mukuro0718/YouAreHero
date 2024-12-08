#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "ParalellNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
ParalellNode::ParalellNode()
	: successCount(0)
	, failureCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
ParalellNode::~ParalellNode()
{

}

/// <summary>
/// 更新
/// </summary>
ParalellNode::NodeState ParalellNode::Update()
{
	/*子をすべて実行*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		if (state == NodeState::SUCCESS)
		{
			this->successCount++;
		}
		else if (state == NodeState::FAILURE)
		{
			this->failureCount++;
		}
	}

	/*判定*/
	if (this->successCount > this->successThreshold)
	{
		return NodeState::SUCCESS;
	}
	if (this->failureCount > this->failureThreshold)
	{
		return NodeState::FAILURE;
	}
	return NodeState::RUNNING;
}