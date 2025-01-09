//=====================================
// @brief　回復エフェクト
//=====================================
#pragma once

class Effect;
class PlayerHealEffect : public Effect
{
public:
	PlayerHealEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerHealEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
};

