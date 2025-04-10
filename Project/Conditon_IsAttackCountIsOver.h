//===========================================================
// @brief 攻撃回数が残っているかを判定するコンディションノード
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Conditon_IsAttackCountIsOver : public ConditionNode
{
public:
	 Conditon_IsAttackCountIsOver();//コンストラクタ
	~Conditon_IsAttackCountIsOver();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
};

