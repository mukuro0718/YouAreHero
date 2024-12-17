//===========================================================
// @brief NORMAL→ANGRYへの状態変化を確認するコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckAngryCondition : public ConditionNode
{
public:
	 CheckAngryCondition();//コンストラクタ
	~CheckAngryCondition();//デストラクタ

	NodeState Update	() override;//更新
};

