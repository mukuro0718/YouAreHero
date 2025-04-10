//=======================================================
// @brief 怒り値を確認するコンディションノード
//=======================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsAngryValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue);//コンストラクタ
	~Condition_IsAngryValueGreaterThanConstant();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新

private:
	const int SPECIFIED_ANGRY_VALUE;//怒り値

};

