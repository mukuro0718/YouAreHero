//=====================================================
// @brief デコレーターノード
// 子のふるまいを変更するノード
// 主にアクションの実行の前後で追加の処理を行ったり、
// 評価結果を変更したりする
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class DecoratorNode abstract: public BehaviorTreeNode
{
public:
			 DecoratorNode();//コンストラクタ
	virtual ~DecoratorNode();//デストラクタ

	virtual NodeState Update	(BehaviorTree& _tree, Character& _chara)abstract;//更新
			void	  AddChild	(BehaviorTreeNode& _child)				override;//子供の追加
protected:
	BehaviorTreeNode* child;
};

