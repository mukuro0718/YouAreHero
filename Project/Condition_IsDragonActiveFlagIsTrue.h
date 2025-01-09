//===========================================================
// @brief アクティブ状態か判定するコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonActiveFlagIsTrue : public ConditionNode
{
public:
	 Condition_IsDragonActiveFlagIsTrue();//コンストラクタ
	~Condition_IsDragonActiveFlagIsTrue();//デストラクタ

	NodeState Update	() override;//更新
};

