//===========================================================
// @brief 特殊攻撃が行えるかの判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckSpecialNodeCondition : public ConditionNode
{
public:
	 CheckSpecialNodeCondition();//コンストラクタ
	~CheckSpecialNodeCondition();//デストラクタ

	NodeState Update	() override;//更新
};

