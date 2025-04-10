#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "CompositeNode.h"
#include "RandomSelector.h"

/// <summary>
/// コンストラクタ
/// </summary>
RandomSelector::RandomSelector()
{

}

/// <summary>
/// デストラクタ
/// </summary>
RandomSelector::~RandomSelector()
{

}

/// <summary>
/// 更新
/// </summary>
BehaviorTreeNode::NodeState RandomSelector::Update(BehaviorTree& _tree, Character& _chara)
{
	/*実行する子供の番号を取得*/
	int random = GetRand(this->children.size() - 1);

	/*子を実行*/
	NodeState state = this->children[random]->Update(_tree, _chara);
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
	return NodeState::FAILURE;

}