//===========================================================
// @brief ドラゴンの段階が異なっていたら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsStageIsDifferent : public ConditionNode
{
public:
	 Condition_IsStageIsDifferent();//コンストラクタ
	~Condition_IsStageIsDifferent();//デストラクタ

	NodeState Update	() override;//更新
};

