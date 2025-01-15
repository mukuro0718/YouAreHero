//===========================================================
// @brief バトルアクションが選択されているか
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsSelectedDragonBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedDragonBattleAction();//コンストラクタ
	~Condition_IsSelectedDragonBattleAction();//デストラクタ

	NodeState Update	() override;//更新
};

