//=====================================================
// @brief 右足攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_RightFootAttack : public ActionNode
{
public:
	 Beast_RightFootAttack();//コンストラクタ
	~Beast_RightFootAttack();//デストラクタ

	NodeState Update()override;//更新
};

