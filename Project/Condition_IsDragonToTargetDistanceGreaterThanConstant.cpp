#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::Condition_IsDragonToTargetDistanceGreaterThanConstant(const float _compareDistance)
	: COMPARE_DISTANCE(_compareDistance)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::~Condition_IsDragonToTargetDistanceGreaterThanConstant()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonToTargetDistanceGreaterThanConstant::NodeState Condition_IsDragonToTargetDistanceGreaterThanConstant::Update()
{
	/*目標との距離が定数以上だったら*/
	auto& root = Singleton<DragonBehaviorTree>::GetInstance();
	if (root.GetToTargetDistance() >= this->COMPARE_DISTANCE)
	{
		//printfDx("CONDITION_DISTANCE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_DISTANCE_FAILURE\n");
	return NodeState::FAILURE;
}
