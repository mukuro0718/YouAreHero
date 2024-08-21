//===========================================================================
//@brief FPSコントローラークラス
// FPSの設定、計算と描画を行う
// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q10175479905より引用
//===========================================================================
#pragma once

class FPSController
{
public:
	FPSController();
	~FPSController();
	void Initialize();		//初期化
	void Average();			//FPSの平均を算出
	void Wait();			//目標FPSになるよう待機
	void UpdateTargetFPS(); //目標FPSの更新
private:
	/*メンバ変数*/
	double	fps;		//fps
	int		startTime;//計測開始時間
	int		count;	//処理回数
	int		targetFPS;
};