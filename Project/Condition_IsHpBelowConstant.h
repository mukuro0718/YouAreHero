//===========================================================
// @brief HPが定数以下だったら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsHpBelowConstant : public ConditionNode
{
public:
	 Condition_IsHpBelowConstant(const int _specifiedHp);//コンストラクタ
	~Condition_IsHpBelowConstant();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int SPECIFIED_HP;//HP
};

