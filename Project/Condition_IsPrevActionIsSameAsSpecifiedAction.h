//===========================================================
// @brief 前のアクションが、評価したいアクションと同じか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsPrevActionIsSameAsSpecifiedAction : public ConditionNode
{
public:
	 Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction);//コンストラクタ
	~Condition_IsPrevActionIsSameAsSpecifiedAction();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int SPECIFIED_ACTION;
};

