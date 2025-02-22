//=====================================================
// @brief ランダムセレクターノード
// 子ノードをランダムで選択する
//=====================================================
#pragma once

class DecoratorNode;
class Decorator_Inverse : public DecoratorNode
{
public:
			 Decorator_Inverse();//コンストラクタ
	virtual ~Decorator_Inverse();//デストラクタ

	NodeState Update() override;//更新
};

