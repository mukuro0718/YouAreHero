//=====================================================
// @brief パラレルノード
// 子を順番に評価し、すべての子ノードが成功するまで実行する。
// いずれかの子ノードが失敗したら即座に失敗を返す
//=====================================================
#pragma once

class BehaviorTree;
class CompositeNode;
class Character;
class ParalellNode : public CompositeNode
{
public:
	 ParalellNode();//コンストラクタ
	~ParalellNode();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;
protected:
	/*メンバ変数*/
	int successThreshold;//成功しきい値
	int failureThreshold;//失敗しきい値
	int successCount;//成功した数
	int failureCount;//失敗した数
};

