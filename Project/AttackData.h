#pragma once
class AttackData abstract
{
public:
	AttackData();
	virtual ~AttackData(){}

	VECTOR hitPosition;
	bool isDoHitCheck;//当たり判定をするか
	bool isHitAttack;//攻撃が当たったか
	int damage;//ダメージ
	//ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	//攻撃側のヒットストップは、攻撃クラスで設定する。
	int hitStopTime;
	int hitStopType;
	int hitStopDelay;
	float slowFactor;
};

