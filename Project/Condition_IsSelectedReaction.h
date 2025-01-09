//===========================================================
// @brief リアクションが選択されているか
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedReaction : public ConditionNode
{
public:
	 Condition_IsSelectedReaction();//コンストラクタ
	~Condition_IsSelectedReaction();//デストラクタ

	NodeState Update	() override;//更新
};

