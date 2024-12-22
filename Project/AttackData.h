//=======================================================
// @brief 攻撃データクラス
//=======================================================
#pragma once

class AttackData
{
public:
			 AttackData();	//コンストラクタ
	virtual ~AttackData(){}	//デストラクタ

	VECTOR	hitPosition;	//ヒット座標
	bool	isDoHitCheck;	//当たり判定をするか
	bool	isHitAttack;	//攻撃が当たったか
	int		damage;			//ダメージ
	int		hitStopTime;	//ヒットストップ時間
	int		hitStopType;	//ヒットストップの種類
	int		hitStopDelay;	//ヒットストップディレイ
	float	slowFactor;		//スロウ
	int		reactionType;
	/*
	HACK:
	ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	攻撃側のヒットストップは、攻撃クラスで設定する。
	*/
};

