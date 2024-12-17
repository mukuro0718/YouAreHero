//=====================================================
// @brief エクスキュートオールノード
// 子をすべて実行する。
// すべての子ノードが成功したら即座に完了を返す
//=====================================================
#pragma once

class CompositeNode;
class ExecuteAllNode : public CompositeNode
{
public:
	 ExecuteAllNode();//コンストラクタ
	~ExecuteAllNode();//デストラクタ

	NodeState Update() override;
};

