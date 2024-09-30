//=====================================
// @brief　プレイヤーエントリーエフェクト
//=====================================
#pragma once

class Effect;
class PlayerEntryEffect : public Effect
{
public:
	 PlayerEntryEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerEntryEffect(){}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

