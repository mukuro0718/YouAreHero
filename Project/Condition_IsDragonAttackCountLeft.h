//===========================================================
// @brief 残り攻撃回数が０か
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonAttackCountLeft : public ConditionNode
{
public:
	 Condition_IsDragonAttackCountLeft();//コンストラクタ
	~Condition_IsDragonAttackCountLeft();//デストラクタ

	NodeState Update	() override;//更新
};

