//========================================================================
// @brief アクションのインターバルが終了しているかを確認するコンディションノード
//========================================================================
#pragma once

class ConditionNode;
class Condition_IsDragonActionIntervalIsOver : public ConditionNode
{
public:
	 Condition_IsDragonActionIntervalIsOver(const int _actionIndex);//コンストラクタ
	~Condition_IsDragonActionIntervalIsOver();//デストラクタ

	NodeState Update	() override;//更新
private:
	const int ACTION_INDEX;//アクション番号
};

