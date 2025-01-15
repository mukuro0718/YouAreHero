//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class ActionNode;
class Dragon_PlayCurrentBattleAction : public ActionNode
{
public:
	 Dragon_PlayCurrentBattleAction();//コンストラクタ
	~Dragon_PlayCurrentBattleAction();//デストラクタ

	NodeState Update()override;//更新
};

