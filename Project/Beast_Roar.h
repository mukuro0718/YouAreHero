//=====================================================
// @brief 咆哮アクション
// 通常→怒りへの状態変化を行うときの咆哮
//=====================================================
#pragma once

class ActionNode;
class Beast_Roar : public ActionNode
{
public:
	 Beast_Roar();//コンストラクタ
	~Beast_Roar();//デストラクタ

	NodeState Update()override;//更新
};

