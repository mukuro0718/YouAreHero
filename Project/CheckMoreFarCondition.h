//===========================================================
// @brief 歩くかの判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckMoreFarCondition : public ConditionNode
{
public:
	 CheckMoreFarCondition();//コンストラクタ
	~CheckMoreFarCondition();//デストラクタ

	NodeState Update	() override;//更新
};

