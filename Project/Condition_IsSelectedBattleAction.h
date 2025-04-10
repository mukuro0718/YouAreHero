//===========================================================
// @brief バトルアクションが選択されているか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsSelectedBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedBattleAction();//コンストラクタ
	~Condition_IsSelectedBattleAction();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
};

