//===========================================================
// @brief HPが定数以下か
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonHpIsLessThanConstant : public ConditionNode
{
public:
	 Condition_IsDragonHpIsLessThanConstant(const int _compareHp);//コンストラクタ
	~Condition_IsDragonHpIsLessThanConstant();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int COMPARE_HP;//比較したい体力
};

