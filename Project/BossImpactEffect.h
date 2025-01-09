//=====================================
// @brief　ボス攻撃ヒット時エフェクト
//=====================================
#pragma once

class Effect;
class BossImpactEffect : public Effect
{
public:
	BossImpactEffect(const int _effectResourceHandle); //コンストラクタ
	~BossImpactEffect() {}								//デストラクタ

	void Update() override;//更新
private:
	float	yOffset;			//Yオフセット
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
};
