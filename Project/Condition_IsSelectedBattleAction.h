//===========================================================
// @brief バトルアクションが選択されているか
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedBattleAction();//コンストラクタ
	~Condition_IsSelectedBattleAction();//デストラクタ

	NodeState Update	() override;//更新
};

