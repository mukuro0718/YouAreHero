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
CheckRushCondition::CheckRushCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckRushCondition::~CheckRushCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckRushCondition::NodeState CheckRushCondition::Update()
{
	/*状態がNORMAL出なければFAILUREを返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*移動目標までの距離を求める*/
	auto& enemyManager = Singleton<EnemyManager>::GetInstance();
	auto& enemy = dynamic_cast<Enemy&>(enemyManager.GetCharacter());
	float distance = VSize(VSub(enemy.GetMoveTarget(), enemy.GetRigidbody().GetPosition()));

	/*距離がreyの範囲内だったら失敗*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (rootNode.GetBeastState() != BeastBehaviorTree::BeastState::ANGRY) return NodeState::FAILURE;
	if (distance < json.GetJson(JsonManager::FileType::BEAST)["RAY_RANGE"]) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}