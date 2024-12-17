#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "ExecuteAllNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
ExecuteAllNode::ExecuteAllNode()
{

}

/// <summary>
/// デストラクタ
/// </summary>
ExecuteAllNode::~ExecuteAllNode()
{

}

/// <summary>
/// 更新
/// </summary>
ExecuteAllNode::NodeState ExecuteAllNode::Update()
{
	int successCount = 0;
	/*すべての子を実行する*/
	for (auto& child : this->children)
	{
		//成功していたらカウントを増やす
		if (child->Update() == NodeState::SUCCESS)
		{
			successCount++;
		}
	}

	/*子がすべて成功したら完了を返す*/
	if (successCount == this->children.size())
	{
		return NodeState::COMPLETED;
	}
	return NodeState::RUNNING;
}