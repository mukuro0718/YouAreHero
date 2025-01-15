//=====================================================
// @brief なぎ払い＋ブレス＋叩きつけアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_SweepBreathSmash : public ActionNode
{
public:
	 Dragon_SweepBreathSmash();//コンストラクタ
	~Dragon_SweepBreathSmash();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK_1	= 0,//歩き
		SWEEP	= 1,//なぎ払い
		BREATH	= 2,//ブレス
		WALK_2	= 3,//歩き
		SMASH	= 4,//叩きつけ
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
};

