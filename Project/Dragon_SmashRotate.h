//=====================================================
// @brief 叩きつけ＋回転攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_SmashRotate : public ActionNode
{
public:
	 Dragon_SmashRotate();//コンストラクタ
	~Dragon_SmashRotate();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK	= 0,//歩き
		SMASH	= 1,//叩きつけ
		ROTATE	= 2,//回転攻撃
	};

	/*メンバ変数*/
	vector<short>	animationType;		//アニメーションの種類
	vector<float>	animationPlayTime;	//アニメーション再生時間
	short			useAnimationType;	//使用するアニメーションのタイプ
	short			maxUseAnimation;	//使用するアニメーションの最大
	float			nearAttackRange;	//近接攻撃範囲
	float			smashCancelPlayTime;//叩きつけ攻撃キャンセルフレーム
	float			totalPlayTime;		//総再生時間
};

