//=====================================================
// @brief 後ろに下がりながら咆哮アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_BackingBreath : public ActionNode
{
public:
	 Beast_BackingBreath();//コンストラクタ
	~Beast_BackingBreath();//デストラクタ

	NodeState Update()override;//更新
};

