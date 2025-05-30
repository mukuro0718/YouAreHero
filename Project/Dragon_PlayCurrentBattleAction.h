//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_PlayCurrentBattleAction : public ActionNode
{
public:
	 Dragon_PlayCurrentBattleAction();//コンストラクタ
	~Dragon_PlayCurrentBattleAction();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
};

