//=============================================
// @brief チュートリアル管理クラス
// チュートリアルの進行度をチェックする
//=============================================
#pragma once
#include "Singleton.h"

class TutorialManager : public Singleton<TutorialManager>
{
public:
	friend class Singleton<TutorialManager>;
	
	/*エネミーの種類*/
	enum class Stage
	{
		CAMERA	= 0,
		MOVE	= 1,
		ATTACK	= 2,
		AVOID	= 3,
		HEAL	= 4,
		BATTLE	= 5,
	};

	void Initialize	();//初期化
	void Update		();//更新

	/*getter*/
	const int  GetStage()const { return this->stage; }
private:
	 TutorialManager();
	~TutorialManager();

	/*メンバ変数*/
	int  stage;				//現在のチュートリアルの進行度
	bool isMoveCamera;		//アクションフラグ
	bool isLockon;
	bool isWalk;
	bool isRun;
	bool isWeakAttack;
	bool isRotateAttack;
	bool isAvoid;
	bool isBlock;
	bool isHeal;
};

