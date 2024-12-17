//===========================================================
// @brief 両足溜め攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckChargeBothAttackCondition : public ConditionNode
{
public:
	 CheckChargeBothAttackCondition();//コンストラクタ
	~CheckChargeBothAttackCondition();//デストラクタ

	NodeState Update	() override;//更新
};

