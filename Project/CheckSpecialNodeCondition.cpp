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
CheckSpecialNodeCondition::CheckSpecialNodeCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckSpecialNodeCondition::~CheckSpecialNodeCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckSpecialNodeCondition::NodeState CheckSpecialNodeCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (rootNode.GetInterval(static_cast<int>(BeastBehaviorTree::ActionType::RAISE_LEVEL)) != 0) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}