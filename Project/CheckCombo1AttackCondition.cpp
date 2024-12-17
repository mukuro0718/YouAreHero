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
CheckCombo1AttackCondition::CheckCombo1AttackCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckCombo1AttackCondition::~CheckCombo1AttackCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckCombo1AttackCondition::NodeState CheckCombo1AttackCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::COMBO_ATTACK_1)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}