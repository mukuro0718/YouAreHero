//=====================================================
// @brief ブレス＋叩きつけ＋なぎ払い＋回転攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmashSweepRotate : public ActionNode
{
public:
	 Dragon_BreathSmashSweepRotate();//コンストラクタ
	~Dragon_BreathSmashSweepRotate();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		BREATH = 0,//ブレス
		WALK   = 1,//歩き
		SMASH  = 2,//叩きつけ
		SWEEP  = 3,//なぎ払い
		ROTATE = 4,//回転攻撃
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

