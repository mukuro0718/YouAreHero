#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "DecoratorNode.h"
#include "Decorator_Inverse.h"

/// <summary>
/// コンストラクタ
/// </summary>
Decorator_Inverse::Decorator_Inverse()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Decorator_Inverse::~Decorator_Inverse()
{

}

/// <summary>
/// 更新
/// </summary>
BehaviorTreeNode::NodeState Decorator_Inverse::Update(BehaviorTree& _tree, Character& _chara)
{
	/*子を実行*/
	NodeState state = this->child->Update(_tree, _chara);
	//状態が成功なら成功を返し終了
	if (state == NodeState::SUCCESS)
	{
		//printfDx("SELECTOR_SUCCESS\n");
		return NodeState::FAILURE;
	}
	//状態が実行中なら実行中を返し終了
	if (state == NodeState::FAILURE)
	{
		//printfDx("SELECTOR_RUNNING\n");
		return NodeState::SUCCESS;
	}
	//printfDx("SELECTOR_FAILURE\n");
	return NodeState::FAILURE;

}