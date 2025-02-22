//===========================================================
// @brief 攻撃回数が残っているかを判定するコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class Conditon_IsAttackCountIsOver : public ConditionNode
{
public:
	 Conditon_IsAttackCountIsOver();//コンストラクタ
	~Conditon_IsAttackCountIsOver();//デストラクタ

	NodeState Update	() override;//更新
};

