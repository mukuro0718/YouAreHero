//=====================================================
// @brief ブレス＋叩きつけアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmash : public ActionNode
{
public:
	 Dragon_BreathSmash();//コンストラクタ
	~Dragon_BreathSmash();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		BREATH	= 0,//ブレス
		WALK	= 1,//歩き
		SMASH	= 2,//叩きつけ
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
};

