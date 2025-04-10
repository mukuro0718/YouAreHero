//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_PlayCurrentBattleAction : public ActionNode
{
public:
	 Brawler_PlayCurrentBattleAction();//コンストラクタ
	~Brawler_PlayCurrentBattleAction();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

