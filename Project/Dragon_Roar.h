//=====================================================
// @brief 咆哮アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Roar : public ActionNode
{
public:
	 Dragon_Roar();//コンストラクタ
	~Dragon_Roar();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
private:
	short frameCount;	//フレームカウント
	short roarFrame;	//咆哮のフレーム
};

