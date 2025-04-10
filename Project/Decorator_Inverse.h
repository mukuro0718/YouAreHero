//=====================================================
// @brief ランダムセレクターノード
// 子ノードをランダムで選択する
//=====================================================
#pragma once

class BehaviorTree;
class DecoratorNode;
class Character;
class Decorator_Inverse : public DecoratorNode
{
public:
			 Decorator_Inverse();//コンストラクタ
	virtual ~Decorator_Inverse();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//更新
};

