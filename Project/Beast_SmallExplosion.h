//=====================================================
// @brief 前方に小爆発アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_SmallExplosion : public ActionNode
{
public:
	 Beast_SmallExplosion();//コンストラクタ
	~Beast_SmallExplosion();//デストラクタ

	NodeState Update()override;//更新
};

