//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_PlayCurrentBattleAction : public ActionNode
{
public:
	 Beast_PlayCurrentBattleAction();//コンストラクタ
	~Beast_PlayCurrentBattleAction();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

