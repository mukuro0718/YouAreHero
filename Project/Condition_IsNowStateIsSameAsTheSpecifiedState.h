//===========================================================
// @brief ボスの状態が評価したい状態と同じか
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsNowStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsNowStateIsSameAsTheSpecifiedState(const int _specifiedState);//コンストラクタ
	~Condition_IsNowStateIsSameAsTheSpecifiedState();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int SPECIFIED_STATE;
};

