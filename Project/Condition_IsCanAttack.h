//===========================================================
// @brief 攻撃権があるか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsCanAttack : public ConditionNode
{
public:
	 Condition_IsCanAttack();//コンストラクタ
	~Condition_IsCanAttack();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
};

