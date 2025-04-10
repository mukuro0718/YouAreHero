//================================================================================
// @brief 指定のアクションのインターバルが終了しているかを判定するコンディションノード
//================================================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsActionIntervalIsOver : public ConditionNode
{
public:
	 Condition_IsActionIntervalIsOver(const int _actionIndex);//コンストラクタ
	~Condition_IsActionIntervalIsOver();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
private:
	const int ACTION_INDEX;//アクション番号
};

