//=====================================================
// @brief シーケンスノード
// 子を順番に評価し、すべての子ノードが成功するまで実行する。
// いずれかの子ノードが失敗したら即座に失敗を返す
//=====================================================
#pragma once

class CompositeNode;
class SequencerNode : public CompositeNode
{
	 SequencerNode();//コンストラクタ
	~SequencerNode();//デストラクタ

	NodeState Update() override;
};

