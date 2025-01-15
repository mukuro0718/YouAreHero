//=====================================================
// @brief ランダムセレクターノード
// 子ノードをランダムで選択する
//=====================================================
#pragma once

class CompositeNode;
class RandomSelector : public CompositeNode
{
public:
			 RandomSelector();//コンストラクタ
	virtual ~RandomSelector();//デストラクタ

	NodeState Update() override;//更新
};

