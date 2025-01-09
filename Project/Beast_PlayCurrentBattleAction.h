//=====================================================
// @brief 登録しているアクションを実行
//=====================================================
#pragma once

class ActionNode;
class Beast_PlayCurrentBattleAction : public ActionNode
{
public:
	 Beast_PlayCurrentBattleAction();//コンストラクタ
	~Beast_PlayCurrentBattleAction();//デストラクタ

	NodeState Update()override;//更新
};

