//=====================================================
// @brief 咆哮アクション
// 通常→怒りへの状態変化を行うときの咆哮
//=====================================================
#pragma once

class ActionNode;
class Beast_FootComboAttack : public ActionNode
{
public:
	 Beast_FootComboAttack();//コンストラクタ
	~Beast_FootComboAttack();//デストラクタ

	NodeState Update()override;//更新
};

