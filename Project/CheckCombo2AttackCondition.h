//===========================================================
// @brief コンボ２攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckCombo2AttackCondition : public ConditionNode
{
public:
	 CheckCombo2AttackCondition();//コンストラクタ
	~CheckCombo2AttackCondition();//デストラクタ

	NodeState Update	() override;//更新
};

