//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Idle : public ActionNode
{
public:
	 Dragon_Idle();//コンストラクタ
	~Dragon_Idle();//デストラクタ

	NodeState Update()override;//更新
};

