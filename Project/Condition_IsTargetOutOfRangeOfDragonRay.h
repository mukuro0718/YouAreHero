//===========================================================
// @brief 目標がRayの外だったら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsTargetOutOfRangeOfDragonRay : public ConditionNode
{
public:
	 Condition_IsTargetOutOfRangeOfDragonRay(const float _maxToleranceRange, const float _minToleranceRange);//コンストラクタ
	~Condition_IsTargetOutOfRangeOfDragonRay();//デストラクタ

	NodeState Update	() override;//更新
private:
	const float MAX_TOLERANCE_RANGE;//最小許容範囲
	const float MIN_TOLERANCE_RANGE;//最大許容範囲
};

