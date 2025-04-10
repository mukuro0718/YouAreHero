#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfRay::Condition_IsTargetOutOfRangeOfRay(const float _maxToleranceRange, const float _minToleranceRange)
	: MAX_TOLERANCE_RANGE(_maxToleranceRange)
	, MIN_TOLERANCE_RANGE(_minToleranceRange)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsTargetOutOfRangeOfRay::~Condition_IsTargetOutOfRangeOfRay()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsTargetOutOfRangeOfRay::NodeState Condition_IsTargetOutOfRangeOfRay::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�ڕW�Ƃ̋������萔�ȏゾ������*/
	float dot = _tree.GetDotOfDirAndToTarget();
	if (this->MIN_TOLERANCE_RANGE <= dot && dot <= this->MAX_TOLERANCE_RANGE)
	{
		//printfDx("CONDITION_DOT_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_DOT_SUCCESS\n");
	return NodeState::SUCCESS;
}
