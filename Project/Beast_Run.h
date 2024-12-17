//=====================================================
// @brief ダッシュアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Run : public ActionNode
{
public:
	 Beast_Run();//コンストラクタ
	~Beast_Run();//デストラクタ

	NodeState Update()override;//更新
};

