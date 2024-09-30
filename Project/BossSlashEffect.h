//=====================================
// @brief　ボス切りつけエフェクト
//=====================================
#pragma once

class Effect;
class BossSlashEffect : public Effect
{
public:
	BossSlashEffect(const int _effectResourceHandle);//コンストラクタ
	~BossSlashEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

