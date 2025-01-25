//=====================================================
// @brief 回転攻撃＋なぎ払い＋叩きつけアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_RotateSweepSmash : public ActionNode
{
public:
	 Dragon_RotateSweepSmash();//コンストラクタ
	~Dragon_RotateSweepSmash();//デストラクタ

	NodeState Update()override;//更新
private:
	/*列挙体*/
	//アニメーションの種類
	enum class UseAnimationType
	{
		WALK_1	= 0,//歩き
		ROTATE	= 1,//回転攻撃
		WALK_2	= 2,//歩き
		SWEEP	= 3,//なぎ払い
		SMASH	= 4,//叩きつけ
	};

	/*定数*/
	const int FIX_ROTATE_FRAME;//回転補正フレーム

	/*メンバ変数*/
	vector<short>	 animationType;		 //アニメーションの種類
	vector<float>	 animationPlayTime;	 //アニメーション再生時間
	short			 useAnimationType;	 //使用するアニメーションのタイプ
	short			 maxUseAnimation;	 //使用するアニメーションの最大
	float			 nearAttackRange;	 //近接攻撃範囲
	float			 sweepCancelPlayTime;//なぎ払い攻撃キャンセルフレーム
	map<short, short>useColliderIndex;	 //使用するコライダー
	float			 nowTotalPlayTime;	 //現在の総再生時間
	bool			 isClose;			 //近づいたか
	short			 frameCount;		 //フレームカウント
};

