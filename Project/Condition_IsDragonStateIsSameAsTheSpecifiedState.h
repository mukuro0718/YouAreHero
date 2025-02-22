//===========================================================
// @brief ドラゴンの状態を評価
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsDragonStateIsSameAsTheSpecifiedState(const int _specifiedState);//コンストラクタ
	~Condition_IsDragonStateIsSameAsTheSpecifiedState();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int SPECIFIED_STATE;

};

