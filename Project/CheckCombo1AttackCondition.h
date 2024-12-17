//===========================================================
// @brief コンボ１攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckCombo1AttackCondition : public ConditionNode
{
public:
	 CheckCombo1AttackCondition();//コンストラクタ
	~CheckCombo1AttackCondition();//デストラクタ

	NodeState Update	() override;//更新
};

