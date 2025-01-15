//=====================================================
// @brief なぎ払い＋叩きつけ＋回転攻撃＋ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepSmashRotateBreath : public ActionNode
{
public:
	 Dragon_SweepSmashRotateBreath();//コンストラクタ
	~Dragon_SweepSmashRotateBreath();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK	= 0,//歩き
		SWEEP	= 1,//なぎ払い
		SMASH	= 2,//叩きつけ
		ROTATE	= 3,//回転攻撃
		BREATH	= 4,//ブレス
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
	float			smashCancelPlayTime;//叩きつけ攻撃キャンセルフレーム
	float			sweepCancelPlayTime;//なぎ払い攻撃キャンセルフレーム
	float			totalPlayTime;		//総再生時間

};

