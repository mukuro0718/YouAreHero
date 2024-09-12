//========================================
// @brief アクションパラメータークラス
// いくつかのパラメーターからそのアクションに対する
// 欲求値を決め、それをもとに期待値を出す
//========================================

#pragma once
class ActionParameter
{
public:
	ActionParameter();//コンストラクタ
	~ActionParameter(){}//デストラクタ

	void AddDesireValue(const int _addValue);

	static constexpr int MAX_PARAMETER = 255;
	int desireValue;//欲求値
	int interval;	//インターバル
};

