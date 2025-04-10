//===========================================================
// @brief リアクションが選択されているか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsSelectedReaction : public ConditionNode
{
public:
	 Condition_IsSelectedReaction();//コンストラクタ
	~Condition_IsSelectedReaction();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
};

