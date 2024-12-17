//===========================================================
// @brief スーパーノヴァの判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckSuperNovaCondition : public ConditionNode
{
public:
	 CheckSuperNovaCondition();//コンストラクタ
	~CheckSuperNovaCondition();//デストラクタ

	NodeState Update	() override;//更新
};

