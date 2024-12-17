//===========================================================
// @brief 右足溜め攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckChargeRightAttackCondition : public ConditionNode
{
public:
	 CheckChargeRightAttackCondition();//コンストラクタ
	~CheckChargeRightAttackCondition();//デストラクタ

	NodeState Update	() override;//更新
};

