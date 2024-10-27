//===========================================================================
//@brief FPSコントローラークラス
// FPSの設定、計算と描画を行う
//===========================================================================
#pragma once
#include "Singleton.h"

class FPSController : public Singleton<FPSController>
{
public:
	friend class Singleton<FPSController>;

	void Initialize();		//初期化
	void CalcStartTime();

	void Update();			//FPSの更新
	void UpdateTargetFPS(); //目標FPSの更新
	const void Draw()const;

private:
	static constexpr int NORMAL_TIME_SCALE = 1000;

	FPSController();
	~FPSController();

	/*メンバ変数*/
	int		targetFPS;	//目標FPS
	double	fps;		//fps
	int		startTime;	//計測開始時間
	int		timeScale;	//時間のスケール(1000 = 1秒)
	int		frameCount;	//フレームカウント
	int		lastTime;	//最後の時間
};