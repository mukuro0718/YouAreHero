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
CheckCombo2AttackCondition::CheckCombo2AttackCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckCombo2AttackCondition::~CheckCombo2AttackCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckCombo2AttackCondition::NodeState CheckCombo2AttackCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::RIGHT_FOOT_ATTACK)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}