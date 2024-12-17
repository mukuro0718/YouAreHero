//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Idle : public ActionNode
{
public:
	 Beast_Idle();//コンストラクタ
	~Beast_Idle();//デストラクタ

	NodeState Update()override;//更新
};

