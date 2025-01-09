//=====================================
// @brief　プレイヤーの攻撃ヒットエフェクト
//=====================================
#pragma once

class Effect;
class PlayerImpactEffect : public Effect
{
public:
	PlayerImpactEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerImpactEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル};
};
