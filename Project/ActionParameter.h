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
	bool CalcInterval();
	void SetInterval(const int _set);
	const int GetWeight(const int _sum)const;

	static constexpr float MAX_PARAMETER = 255.0f;
	static constexpr float BASE_ADD_DESIRE_VALUE = 1.0f;
	static constexpr float BASE_WEIGHT = 100.0f;
	float desireValue;//欲求値
	int interval;	//インターバル
};

