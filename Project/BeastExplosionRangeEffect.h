//=====================================
// @brief　獣ボス爆発範囲エフェクト
//=====================================
#pragma once

class Effect;
class BeastExplosionRangeEffect : public Effect
{
public:
	 BeastExplosionRangeEffect(const int _effectResourceHandle);//コンストラクタ
	~BeastExplosionRangeEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
	short	useIndexNum;		//使用するフレームの番号
};

