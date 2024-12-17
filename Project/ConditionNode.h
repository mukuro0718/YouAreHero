//=====================================================
// @brief コンディションノード
// 条件を評価し、成功または失敗を返す。
// 条件が満たされているかどうかをチェックする
// 継承先に詳細な条件を書く
//=====================================================
#pragma once

class BehaviorTreeNode;
class ConditionNode abstract: public BehaviorTreeNode
{
public:
			 ConditionNode(){}//コンストラクタ
	virtual ~ConditionNode(){}//デストラクタ

	virtual NodeState Update() abstract;//更新
};

