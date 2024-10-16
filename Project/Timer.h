//===========================================================================
//@brief タイマークラス
// ゲーム開始時に初期化し、ゲーム中計測をする
//===========================================================================
#pragma once
#include "Singleton.h"

class Timer : public Singleton<Timer>
{
public:
	friend class Singleton<Timer>;

	void Initialize() { this->time = 0; }		//初期化

	void Update() { this->time++; } //目標FPSの更新

	const void Draw() const;

	const int GetNowTime()const { return this->time; }
private:

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*内部処理関数*/
	Timer();
	~Timer(){}

	/*メンバ変数*/
	int		time;
	int fontHandle;
};