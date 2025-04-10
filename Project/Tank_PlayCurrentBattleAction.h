//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_PlayCurrentBattleAction : public ActionNode
{
public:
	 Tank_PlayCurrentBattleAction();//コンストラクタ
	~Tank_PlayCurrentBattleAction();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

