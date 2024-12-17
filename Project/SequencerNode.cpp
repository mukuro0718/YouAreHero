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
	/*子を実行*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update();
		//失敗していたら状態を返す
		if (state == NodeState::FAILURE) return NodeState::FAILURE;
		//実行中なら状態を返す
		if (state == NodeState::RUNNING) return NodeState::RUNNING;
	}
	return NodeState::SUCCESS;
}