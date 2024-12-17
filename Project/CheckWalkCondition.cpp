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
CheckWalkCondition::CheckWalkCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckWalkCondition::~CheckWalkCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckWalkCondition::NodeState CheckWalkCondition::Update()
{
	/*状態がNORMAL出なければFAILUREを返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*移動目標までの距離を求める*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Enemy&>(enemyManager.GetCharacter());
	float distance = VSize(VSub(enemy.GetMoveTarget(), enemy.GetRigidbody().GetPosition()));

	/*距離がreyの範囲内だったら失敗*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (rootNode.GetBeastState() != BeastBehaviorTree::BeastState::NORMAL) return NodeState::FAILURE;
	if (distance < json.GetJson(JsonManager::FileType::BEAST)["RAY_RANGE"]) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}