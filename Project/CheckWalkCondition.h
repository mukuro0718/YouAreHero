//===========================================================
// @brief 歩くかの判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckWalkCondition : public ConditionNode
{
public:
	 CheckWalkCondition();//コンストラクタ
	~CheckWalkCondition();//デストラクタ

	NodeState Update	() override;//更新
};

