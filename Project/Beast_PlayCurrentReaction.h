//=====================================================
// @brief 保存したアクションを実行
//=====================================================
#pragma once

class ActionNode;
class Beast_PlayCurrentReaction : public ActionNode
{
public:
	 Beast_PlayCurrentReaction();//コンストラクタ
	~Beast_PlayCurrentReaction();//デストラクタ

	NodeState Update()override;//更新
};

