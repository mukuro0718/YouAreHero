//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Idle : public ActionNode
{
public:
	 Dragon_Idle();//コンストラクタ
	~Dragon_Idle();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
private:
	/*定数*/
	const short STANDBY_TIME;	//待機時間

	/*メンバ変数*/
	short currentStandbyTime;	//現在の待機時間
	short standbyTime;			//待機時間
};

