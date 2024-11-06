//================================================
// @brief キャラクターデータクラス
//================================================
#pragma once

class CharacterData 
{
public:
			 CharacterData();	//コンストラクタ
	virtual ~CharacterData(){}	//デストラクタ

	/*メンバ変数*/
	VECTOR	hitPosition;	//攻撃ヒット座標
	bool	isHit;			//攻撃にヒットしたか
	bool	isGuard;		//ガードフラグ
	bool	isInvinvible;	//無敵フラグ
	int		hp;				//HP
	int		playerReaction;	//プレイヤーのリアクションタイプ
	int		hitStopTime;	//ヒットストップの時間
	int		hitStopType;	//ヒットストップの種類
	int		hitStopDelay;	//ヒットストップディレイ
	float	slowFactor;		//スロウ
	/*
	HACK:
	ここでのヒットストップ系の変数は、
	キャラクター側で行うヒットストップの変数である
	*/
};

