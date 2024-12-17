//=====================================================
// @brief レベル上昇時咆哮アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_RaiseLevel : public ActionNode
{
public:
	 Beast_RaiseLevel();//コンストラクタ
	~Beast_RaiseLevel();//デストラクタ

	NodeState Update()override;//更新
};

