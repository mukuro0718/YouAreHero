//=====================================
// @brief　ボス攻撃ヒット時エフェクト
//=====================================
#pragma once

class Effect;
class BossImpactEffect : public Effect
{
public:
	BossImpactEffect(const int _effectResourceHandle);//コンストラクタ
	~BossImpactEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

