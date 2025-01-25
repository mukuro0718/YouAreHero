//===========================================================
// @brief 前のアクションが、評価したいアクションと同じか
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsPrevActionIsSameAsSpecifiedAction : public ConditionNode
{
public:
	 Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction);//コンストラクタ
	~Condition_IsPrevActionIsSameAsSpecifiedAction();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int SPECIFIED_ACTION;
};

