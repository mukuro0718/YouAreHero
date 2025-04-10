//=====================================================
// @brief コンポジットノード
// 複数の子ノードをグループ化し、それらの動作を管理する
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class CompositeNode abstract : public BehaviorTreeNode
{
public:
			 CompositeNode();//コンストラクタ
	virtual ~CompositeNode();//デストラクタ

	virtual NodeState Update  (BehaviorTree& _tree, Character& _chara)  abstract;//更新
			void	  AddChild(BehaviorTreeNode& _child)				override;//子供の追加
protected:
	std::vector<BehaviorTreeNode*> children;
};

