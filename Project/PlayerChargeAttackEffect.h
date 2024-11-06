//=====================================
// @brief　プレイヤーの溜め攻撃エフェクト
//=====================================
#pragma once

class Effect;
class PlayerChargeAttackEffect : public Effect
{
public:
	PlayerChargeAttackEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerChargeAttackEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

