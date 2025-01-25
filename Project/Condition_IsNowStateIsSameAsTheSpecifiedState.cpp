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
Condition_IsNowStateIsSameAsTheSpecifiedState::Condition_IsNowStateIsSameAsTheSpecifiedState(const BeastBehaviorTree::BeastState _specifiedState)
	: SPECIFIED_STATE(_specifiedState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsNowStateIsSameAsTheSpecifiedState::~Condition_IsNowStateIsSameAsTheSpecifiedState()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsNowStateIsSameAsTheSpecifiedState::NodeState Condition_IsNowStateIsSameAsTheSpecifiedState::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*状態が指定の状態でなければFAILUREを返す*/
	if (rootNode.GetBeastState() != this->SPECIFIED_STATE)
	{
		//printfDx("CONDITION_STATE_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_STATE_SUCCESS\n");
	return NodeState::SUCCESS;
}