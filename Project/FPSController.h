//===========================================================================
//@brief FPSコントローラークラス
// FPSの設定、計算と描画を行う
// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q10175479905より引用
//===========================================================================
#pragma once
#include "Singleton.h"

class FPSController : public Singleton<FPSController>
{
public:
	friend class Singleton<FPSController>;

	void Initialize();		//初期化
	void CalcStartTime();

	void Average();			//FPSの平均を算出
	void Wait();			//目標FPSになるよう待機
	void UpdateTargetFPS(); //目標FPSの更新
	const void Draw()const;

private:
	static constexpr int NORMAL_TIME_SCALE = 1000;

	FPSController();
	~FPSController();

	/*メンバ変数*/
	int type;
	int		targetFPS;
	double	fps;		//fps
	int		startTime;//計測開始時間
	int		count;	//処理回数
	bool	isDebug;
	int timeScale;
};