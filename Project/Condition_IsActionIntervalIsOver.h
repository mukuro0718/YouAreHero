//===========================================================
// @brief 両足溜め攻撃判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsActionIntervalIsOver : public ConditionNode
{
public:
	 Condition_IsActionIntervalIsOver(const int _actionIndex);//コンストラクタ
	~Condition_IsActionIntervalIsOver();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int ACTION_INDEX;//アクション番号
};

