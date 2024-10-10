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

	bool CalcInterval  ();
	
	/*getter/setter*/
	void	  SetInterval(const int _set);
	const int GetWeight	 (const int _sum)const;

	/*静的定数*/
	static constexpr float BASE_WEIGHT			 = 100.0f;	//
	
	/*メンバ変数*/
	float	desireValue;//欲求値
	int		interval;	//インターバル
};

