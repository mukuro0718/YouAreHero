//===========================================================
// @brief 目標との距離が定数以上だったら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsToTargetDistanceGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsToTargetDistanceGreaterThanConstant(const float _compareDistance);//コンストラクタ
	~Condition_IsToTargetDistanceGreaterThanConstant();//デストラクタ

	NodeState Update	() override;//更新
private:
	const float COMPARE_DISTANCE;//比較したい定数距離
};

