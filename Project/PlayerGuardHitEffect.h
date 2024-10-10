//=====================================
// @brief　ガード時ヒットエフェクト
//=====================================
#pragma once

class Effect;
class PlayerGuardHitEffect : public Effect
{
public:
	PlayerGuardHitEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerGuardHitEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

