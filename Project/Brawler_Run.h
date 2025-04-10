//=====================================================
// @brief ダッシュアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Run : public ActionNode
{
public:
	 Brawler_Run();//コンストラクタ
	~Brawler_Run();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

