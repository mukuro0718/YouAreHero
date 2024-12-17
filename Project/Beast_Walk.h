//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Walk : public ActionNode
{
public:
	 Beast_Walk();//コンストラクタ
	~Beast_Walk();//デストラクタ

	NodeState Update()override;//更新
};

