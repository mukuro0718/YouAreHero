//=============================================
// @brief エネミーチェンジャークラス
// 登場するエネミーの変更を行う
//=============================================
#pragma once
#include "Singleton.h"

class EnemyChanger : public Singleton<EnemyChanger>
{
public:
	friend class Singleton<EnemyChanger>;
	
	/*エネミーの種類*/
	enum class EnemyType
	{
		GOLEM	 = 0,//ゴーレム
		BEAST	 = 1,//獣
		DRAGON	 = 2,//ドラゴン
		TUTORIAL = 3,//チュートリアル
	};

	void Initialize();//初期化
	void Update();//

	const int GetEnemyType()const { return this->enemyType; }
	const bool GetIsProvDecide()const { return this->isProvDecide; }
	const bool GetIsFinalDecide()const { return this->isFinalDecide; }
private:
	 EnemyChanger();
	~EnemyChanger();

	/*静的定数*/
	static constexpr int MAX_INPUT_INTERBVAL = 10;//最大入力インターバル

	/*メンバ変数*/
	int	enemyType;				//タイプ
	int selectType;
	int	inputInterval;		//入力インターバル
	bool isProvDecide;		//仮の決定
	bool isFinalDecide;		//最終決定
};

