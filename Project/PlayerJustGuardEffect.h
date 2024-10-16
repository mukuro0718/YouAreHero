//=====================================
// @brief　ジャストガード時エフェクト
//=====================================
#pragma once

class Effect;
class PlayerJustGuardEffect : public Effect
{
public:
	PlayerJustGuardEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerJustGuardEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

