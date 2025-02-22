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
		TUTORIAL = 0,//チュートリアル
		GOLEM	 = 1,//ゴーレム
		DRAGON	 = 2,//ドラゴン
		BEAST	 = 3,//獣
	};

	void Initialize	();//初期化
	void Update		();//更新

	const int  GetEnemyType		()const { return this->enemyType; }
	const bool GetIsProvDecide	()const { return this->isProvDecide; }
	const bool GetIsFinalDecide	()const { return this->isFinalDecide; }
private:
	 EnemyChanger();
	~EnemyChanger();

	/*静的定数*/
	static constexpr int MAX_INPUT_INTERBVAL = 10;//最大入力インターバル

	/*メンバ変数*/
	int	enemyType;			//タイプ
	int selectType;			//選択中の種類
	int	inputInterval;		//入力インターバル
	bool isProvDecide;		//仮の決定
	bool isFinalDecide;		//最終決定
};

