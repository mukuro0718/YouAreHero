#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsTargetOutOfRangeOfRay::Condition_IsTargetOutOfRangeOfRay(const float _maxToleranceRange, const float _minToleranceRange)
	: MAX_TOLERANCE_RANGE(_maxToleranceRange)
	, MIN_TOLERANCE_RANGE(_minToleranceRange)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsTargetOutOfRangeOfRay::~Condition_IsTargetOutOfRangeOfRay()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsTargetOutOfRangeOfRay::NodeState Condition_IsTargetOutOfRangeOfRay::Update()
{
	/*目標との距離が定数以上だったら*/
	auto& root = Singleton<BeastBehaviorTree>::GetInstance();
	float dot = root.GetDotOfDirAndToTarget();
	if (this->MIN_TOLERANCE_RANGE <= dot && dot <= this->MAX_TOLERANCE_RANGE)
	{
		printfDx("CONDITION_DOT_FAILURE\n");
		return NodeState::FAILURE;
	}
	printfDx("CONDITION_DOT_SUCCESS\n");
	return NodeState::SUCCESS;
}
