//===========================================================
// @brief 攻撃を受けているか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsHitAttack : public ConditionNode
{
public:
	 Condition_IsHitAttack();//コンストラクタ
	~Condition_IsHitAttack();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
};

