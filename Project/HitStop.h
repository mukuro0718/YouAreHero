//=============================================
// @ヒットストップクラス
// ストップさせるか、スローにするか
//=============================================

#pragma once
class HitStop final
{
public:
	 HitStop();//コンストラクタ
	~HitStop();//デストラクタ

	void Initialize	();//初期化
	void Update		();//更新
	bool IsHitStop	();//ヒットストップ中か

	/*setter*/
	void SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor);//ヒットストップ時間を設定

	/*列挙体*/
	//種類
	enum class Type
	{
		NONE = -1,
		STOP = 0,
		SLOW = 1
	};
private:
	/*静的定数*/
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;//スロー係数

	/*メンバ変数*/
	float	slowFactor;	//スロー係数
	int		type;		//種類
	int		time;		//ヒットストップ時間
	int		delay;		//待ち時間
};

