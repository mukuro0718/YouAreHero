#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::Condition_IsNodeStateIsSameAsSpecifiedLevel(const BehaviorTreeNode::NodeState _specifiedState)
	: SPECIFIED_STATE(_specifiedState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::~Condition_IsNodeStateIsSameAsSpecifiedLevel()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsNodeStateIsSameAsSpecifiedLevel::NodeState Condition_IsNodeStateIsSameAsSpecifiedLevel::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetNodeState() == this->SPECIFIED_STATE)
	{
		printfDx("CONDITION_NODESTATE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	printfDx("CONDITION_NODESTATE_FAILURE\n");
	return NodeState::FAILURE;
}