//=====================================================
// @brief 弱ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_WeakBreath : public ActionNode
{
public:
	 Beast_WeakBreath();//コンストラクタ
	~Beast_WeakBreath();//デストラクタ

	NodeState Update()override;//更新
};

