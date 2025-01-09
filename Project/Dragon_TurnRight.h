//=====================================================
// @brief 右回りアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_TurnRight : public ActionNode
{
public:
	 Dragon_TurnRight();//コンストラクタ
	~Dragon_TurnRight();//デストラクタ

	NodeState Update()override;//更新
};

