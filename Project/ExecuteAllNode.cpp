#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
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
ExecuteAllNode::NodeState ExecuteAllNode::Update(BehaviorTree& _tree, Character& _chara)
{
	int successCount = 0;
	/*すべての子を実行する*/
	for (auto& child : this->children)
	{
		//成功していたらカウントを増やす
		if (child->Update(_tree, _chara) == NodeState::SUCCESS)
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