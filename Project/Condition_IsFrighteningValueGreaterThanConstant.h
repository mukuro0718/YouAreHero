//=======================================================
// @brief 怯み値が定数以上か
//=======================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsFrighteningValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue);//コンストラクタ
	~Condition_IsFrighteningValueGreaterThanConstant();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int SPECIFIED_FRIGHTENING_VALUE;//怯み値
};

