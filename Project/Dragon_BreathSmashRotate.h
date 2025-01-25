//=====================================================
// @brief ブレス＋叩きつけ＋回転攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_BreathSmashRotate : public ActionNode
{
public:
	 Dragon_BreathSmashRotate();//コンストラクタ
	~Dragon_BreathSmashRotate();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		BREATH	= 0,//ブレス
		WALK	= 1,//歩き
		SMASH	= 2,//叩きつけ
		ROTATE	= 3,//回転攻撃
	};

	/*定数*/
	const int FIX_ROTATE_FRAME;//回転補正フレーム

	/*メンバ変数*/
	vector<short>	 animationType;		 //アニメーションの種類
	vector<float>	 animationPlayTime;	 //アニメーション再生時間
	short			 useAnimationType;	 //使用するアニメーションのタイプ
	short			 maxUseAnimation;	 //使用するアニメーションの最大
	float			 nearAttackRange;	 //近接攻撃範囲
	float			 smashCancelPlayTime;//叩きつけ攻撃キャンセルフレーム
	map<short, short>useColliderIndex;	 //使用するコライダー
	float			 nowTotalPlayTime;	 //現在の総再生時間
	bool			 isClose;			 //近づいたか
	short			 frameCount;		 //フレームカウント
};

