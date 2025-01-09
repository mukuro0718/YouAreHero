//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Dying : public ActionNode
{
public:
	 Beast_Dying();//コンストラクタ
	~Beast_Dying();//デストラクタ

	NodeState Update()override;//更新
private:
	bool  isInitialize;		//初期化したか
};

