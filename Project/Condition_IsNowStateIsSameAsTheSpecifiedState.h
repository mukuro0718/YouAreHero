//===========================================================
// @brief ボスの状態が評価したい状態と同じか
//===========================================================
#pragma once

class ConditionNode;
class BeastBehaviorTree;
class Condition_IsNowStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsNowStateIsSameAsTheSpecifiedState(const BeastBehaviorTree::BeastState _specifiedState);//コンストラクタ
	~Condition_IsNowStateIsSameAsTheSpecifiedState();//デストラクタ

	NodeState Update() override;//更新
private:
	const BeastBehaviorTree::BeastState SPECIFIED_STATE;
};

