//=====================================================
// @brief ダイアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Dying : public ActionNode
{
public:
	 Dragon_Dying();//コンストラクタ
	~Dragon_Dying();//デストラクタ

	NodeState Update()override;//更新
};

