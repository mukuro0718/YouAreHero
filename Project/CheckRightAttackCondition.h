//===========================================================
// @brief 右足攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckRightAttackCondition : public ConditionNode
{
public:
	 CheckRightAttackCondition();//コンストラクタ
	~CheckRightAttackCondition();//デストラクタ

	NodeState Update	() override;//更新
};

