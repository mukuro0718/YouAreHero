//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_PlayCurrentBattleAction : public ActionNode
{
public:
	 Mage_PlayCurrentBattleAction();//コンストラクタ
	~Mage_PlayCurrentBattleAction();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

