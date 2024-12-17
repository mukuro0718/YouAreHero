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
CheckSuperNovaCondition::CheckSuperNovaCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckSuperNovaCondition::~CheckSuperNovaCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckSuperNovaCondition::NodeState CheckSuperNovaCondition::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*レベルが最大じゃなければFAILUREを返す*/
	if (rootNode.GetLevel() != static_cast<int>(BeastBehaviorTree::LevelStage::Lv4)) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}