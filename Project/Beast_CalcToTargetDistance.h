//=====================================================
// @brief プレイヤーとの距離を求めるアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_CalcToTargetDistance : public ActionNode
{
public:
	 Beast_CalcToTargetDistance();//コンストラクタ
	~Beast_CalcToTargetDistance();//デストラクタ

	NodeState Update()override;//更新
};

