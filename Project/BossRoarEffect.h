//=====================================
// @brief　ボス咆哮時エフェクト
//=====================================
#pragma once

class Effect;
class BossRoarEffect : public Effect
{
public:
	 BossRoarEffect(const int _effectResourceHandle);//コンストラクタ
	~BossRoarEffect() {}//デストラクタ

	void SetInfoToPlayTheEffect() override;
private:
};

