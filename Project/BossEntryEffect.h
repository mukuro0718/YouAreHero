//=====================================
// @brief　ボスエントリーエフェクト
//=====================================
#pragma once

class Effect;
class BossEntryEffect : public Effect
{
public:
	BossEntryEffect(const int _effectResourceHandle);//コンストラクタ
	~BossEntryEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

