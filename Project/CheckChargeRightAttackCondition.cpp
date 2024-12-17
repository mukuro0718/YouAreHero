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
CheckChargeRightAttackCondition::CheckChargeRightAttackCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckChargeRightAttackCondition::~CheckChargeRightAttackCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckChargeRightAttackCondition::NodeState CheckChargeRightAttackCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::CHARGE_RIGHT_ATTACK)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}