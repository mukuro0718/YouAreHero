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

	void SetInfoToPlayTheEffect() override;
private:
};

