//=====================================================
// @brief 咆哮アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Roar : public ActionNode
{
public:
	 Dragon_Roar();//コンストラクタ
	~Dragon_Roar();//デストラクタ

	NodeState Update()override;//更新
};

