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
CheckMoreFarCondition::CheckMoreFarCondition()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CheckMoreFarCondition::~CheckMoreFarCondition()
{

}

/// <summary>
/// 更新
/// </summary>
CheckMoreFarCondition::NodeState CheckMoreFarCondition::Update()
{
	/*状態がNORMAL出なければFAILUREを返す*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*移動目標までの距離を求める*/
	float distance = rootNode.GetToTargetDistance();

	/*距離が遠距離攻撃の範囲内だったら失敗*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (distance < json.GetJson(JsonManager::FileType::BEAST)["APPROACH_DISTANCE"]) return NodeState::FAILURE;

	return NodeState::SUCCESS;
}