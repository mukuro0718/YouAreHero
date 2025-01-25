#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::Condition_IsTargetOutOfRangeOfDragonRay(const float _maxToleranceRange, const float _minToleranceRange)
	: MAX_TOLERANCE_RANGE(_maxToleranceRange)
	, MIN_TOLERANCE_RANGE(_minToleranceRange)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::~Condition_IsTargetOutOfRangeOfDragonRay()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsTargetOutOfRangeOfDragonRay::NodeState Condition_IsTargetOutOfRangeOfDragonRay::Update()
{
	/*目標との距離が定数以上だったら*/
	auto& root = Singleton<DragonBehaviorTree>::GetInstance();
	float dot = root.GetDotOfDirAndToTarget();
	if (this->MIN_TOLERANCE_RANGE <= dot && dot <= this->MAX_TOLERANCE_RANGE)
	{
		//printfDx("CONDITION_DOT_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_DOT_SUCCESS\n");
	return NodeState::SUCCESS;
}
