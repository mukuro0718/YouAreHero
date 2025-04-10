//=====================================================
// @brief シーケンスノード
// 子を順番に評価し、すべての子ノードが成功するまで実行する。
// いずれかの子ノードが失敗したら即座に失敗を返す
//=====================================================
#pragma once

class BehaviorTree;
class CompositeNode;
class Character;
class SequencerNode : public CompositeNode
{
public:
	 SequencerNode();//コンストラクタ
	~SequencerNode();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
};

