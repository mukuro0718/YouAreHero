//=====================================================
// @brief ダウンアクション
// 現在は実装していない
// 必要なら実装する
//=====================================================
#pragma once

class ActionNode;
class Beast_Down : public ActionNode
{
public:
	 Beast_Down();//コンストラクタ
	~Beast_Down();//デストラクタ

	NodeState Update()override;//更新
private:
	bool isInitialize;//初期化したか
};

