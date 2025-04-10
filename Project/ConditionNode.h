//=====================================================
// @brief コンディションノード
// 条件を評価し、成功または失敗を返す。
// 条件が満たされているかどうかをチェックする
// 継承先に詳細な条件を書く
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class ConditionNode abstract: public BehaviorTreeNode
{
public:
			 ConditionNode(){}//コンストラクタ
	virtual ~ConditionNode(){}//デストラクタ

	virtual NodeState Update(BehaviorTree& _tree, Character& _chara) abstract;//更新
};

