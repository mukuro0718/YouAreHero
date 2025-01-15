//=====================================================
// @brief なぎ払い＋叩きつけアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepSmash : public ActionNode
{
public:
	 Dragon_SweepSmash();//コンストラクタ
	~Dragon_SweepSmash();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK  = 0,//歩き
		SWEEP = 1,//なぎ払い
		SMASH = 2,//叩きつけ
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
	float			sweepCancelPlayTime;//なぎ払い攻撃キャンセルフレーム
	float			totalPlayTime;		//総再生時間

};

