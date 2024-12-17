//===========================================================
// @brief 突進攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckRushCondition : public ConditionNode
{
public:
	 CheckRushCondition();//コンストラクタ
	~CheckRushCondition();//デストラクタ

	NodeState Update	() override;//更新
};

