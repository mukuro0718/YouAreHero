//===========================================================
// @brief 右足溜め攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsNowLevelIsSameAsTheSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel);//コンストラクタ
	~Condition_IsNowLevelIsSameAsTheSpecifiedLevel();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int SPECIFIED_LEVEL;
};

