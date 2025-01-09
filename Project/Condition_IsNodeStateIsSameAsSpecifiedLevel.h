//===========================================================
// @brief ノードの状態が評価したい状態と同じか
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsNodeStateIsSameAsSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNodeStateIsSameAsSpecifiedLevel(const BehaviorTreeNode::NodeState _specifiedState);//コンストラクタ
	~Condition_IsNodeStateIsSameAsSpecifiedLevel();//デストラクタ

	NodeState Update	() override;//更新
private:
	const BehaviorTreeNode::NodeState SPECIFIED_STATE;
};

