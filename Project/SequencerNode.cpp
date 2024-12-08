#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "SequencerNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
SequencerNode::SequencerNode()
{

}

/// <summary>
/// デストラクタ
/// </summary>
SequencerNode::~SequencerNode()
{

}

/// <summary>
/// 更新
/// </summary>
SequencerNode::NodeState SequencerNode::Update()
{
	for (auto& child : this->children)
	{
		if (child->Update() == NodeState::FAILURE)
		{
			return NodeState::FAILURE;
		}
	}
	return NodeState::SUCCESS;
}