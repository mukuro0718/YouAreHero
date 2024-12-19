//====================================================
// @brief ブラックボードクラス
// ビヘイビアツリーの状態遷移のための変数を保持する
//====================================================
#pragma once
class Blackboard
{
public:
	VECTOR			 targetPosition;	//目標の座標
	float			 toTargetDistance;	//目標までの距離
	int				 internalState;		//ボスの内的状態
	int				 actionState;		//アクションの状態
	int				 downValue;			//ダウン値
	int				 angryValue;		//怒り値
	int				 currentHp;			//現在のHP
	std::vector<int> interval;			//インターバル
};

