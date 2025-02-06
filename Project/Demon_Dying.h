//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class ActionNode;
class Demon_Dying : public ActionNode
{
public:
	 Demon_Dying();//コンストラクタ
	~Demon_Dying();//デストラクタ

	NodeState Update	()override;//更新
	void	  Initialize()override;//初期化
private:
	bool  isInitialize;		//初期化したか
};

