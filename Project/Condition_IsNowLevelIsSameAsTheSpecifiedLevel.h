//===========================================================
// @brief 右足溜め攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsNowLevelIsSameAsTheSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel);//コンストラクタ
	~Condition_IsNowLevelIsSameAsTheSpecifiedLevel();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int SPECIFIED_LEVEL;
};

