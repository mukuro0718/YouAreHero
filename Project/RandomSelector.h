//=====================================================
// @brief ランダムセレクターノード
// 子ノードをランダムで選択する
//=====================================================
#pragma once

class BehaviorTree;
class CompositeNode;
class Character;
class RandomSelector : public CompositeNode
{
public:
			 RandomSelector();//コンストラクタ
	virtual ~RandomSelector();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
};

