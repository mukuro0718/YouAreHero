//=====================================================
// @brief 右足溜め攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_ChargeRightFootAttack : public ActionNode
{
public:
	Beast_ChargeRightFootAttack();//コンストラクタ
	~Beast_ChargeRightFootAttack();//デストラクタ

	NodeState Update()override;//更新
};

