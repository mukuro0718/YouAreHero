//=====================================================
// @brief 右前足で攻撃後回転攻撃
//=====================================================
#pragma once

class ActionNode;
class Beast_RightFootAndRotAttack : public ActionNode
{
public:
	 Beast_RightFootAndRotAttack();//コンストラクタ
	~Beast_RightFootAndRotAttack();//デストラクタ

	NodeState Update()override;//更新
};

