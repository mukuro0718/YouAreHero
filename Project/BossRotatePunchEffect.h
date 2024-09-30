//=====================================
// @brief　ボスパンチエフェクト
//=====================================
#pragma once

class Effect;
class BossRotatePunchEffect : public Effect
{
public:
	BossRotatePunchEffect(const int _effectResourceHandle);//コンストラクタ
	~BossRotatePunchEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

