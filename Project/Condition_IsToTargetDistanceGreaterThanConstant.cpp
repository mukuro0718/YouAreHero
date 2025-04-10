#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::Condition_IsToTargetDistanceGreaterThanConstant(const float _compareDistance)
	: COMPARE_DISTANCE(_compareDistance)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::~Condition_IsToTargetDistanceGreaterThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsToTargetDistanceGreaterThanConstant::NodeState Condition_IsToTargetDistanceGreaterThanConstant::Update(BehaviorTree& _tree, Character& _chara)
{
	/*目標との距離が定数以上だったら*/
	if (_tree.GetToTargetDistance() >= this->COMPARE_DISTANCE)
	{
		//printfDx("CONDITION_DISTANCE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DISTANCE_FAILURE\n");
	return NodeState::FAILURE;
}
