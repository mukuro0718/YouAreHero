//=============================================
// @ヒットストップベースクラス
// 各ヒットストップクラスはこのクラスを継承
// 純粋仮想関数
// ストップさせるか、スローにするか
//=============================================

#pragma once
class HitStop final
{
public:
	HitStop();//コンストラクタ
	~HitStop();//デストラクタ

	void Initialize();
	void Update();
	bool IsHitStop();//ヒットストップ中か

	enum class Type
	{
		NONE,
		STOP,
		SLOW
	};

	void SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor);//ヒットストップ時間を設定

private:
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;

	/*メンバ変数*/
	int type;
	int time;//ヒットストップ時間
	int delay;//待ち時間
	float slowFactor;
};

