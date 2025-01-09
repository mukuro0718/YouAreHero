//=====================================
// @brief　獣ボス爆発エフェクト
//=====================================
#pragma once

class Effect;
class BeastExplosionEffect : public Effect
{
public:
	 BeastExplosionEffect(const int _effectResourceHandle);//コンストラクタ
	~BeastExplosionEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
	short	useIndexNum;		//使用するフレームの番号
};

