//=====================================================
// @brief 左周りアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_TurnLeft : public ActionNode
{
public:
	 Dragon_TurnLeft();//コンストラクタ
	~Dragon_TurnLeft();//デストラクタ

	NodeState Update()override;//更新
};

