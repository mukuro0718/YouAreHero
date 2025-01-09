//===========================================================
// @brief HPが０以下か
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonHpIsLessThanZero : public ConditionNode
{
public:
	 Condition_IsDragonHpIsLessThanZero();//コンストラクタ
	~Condition_IsDragonHpIsLessThanZero();//デストラクタ

	NodeState Update	() override;//更新
};

