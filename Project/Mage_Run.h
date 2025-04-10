//=====================================================
// @brief ダッシュアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Run : public ActionNode
{
public:
	 Mage_Run();//コンストラクタ
	~Mage_Run();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

