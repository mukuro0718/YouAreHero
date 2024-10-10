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

	void SetInfoToPlayTheEffect() override;
private:
};

