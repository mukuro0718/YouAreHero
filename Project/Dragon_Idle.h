//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Idle : public ActionNode
{
public:
	 Dragon_Idle();//コンストラクタ
	~Dragon_Idle();//デストラクタ

	NodeState Update()override;//更新
private:
	/*定数*/
	const short AWAKENING_STANDBY_TIME;	//待機時間
	const short FURY_STANDBY_TIME;		//待機時間
	const short RAMPAGE_STANDBY_TIME;	//待機時間

	/*メンバ変数*/
	short currentStandbyTime;	//現在の待機時間
	short standbyTime;			//待機時間
};

