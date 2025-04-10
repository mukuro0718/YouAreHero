//===========================================================
// @brief HPが定数以下だったら
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsHpBelowConstant : public ConditionNode
{
public:
	 Condition_IsHpBelowConstant(const int _specifiedHp);//コンストラクタ
	~Condition_IsHpBelowConstant();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int SPECIFIED_HP;//HP
};

