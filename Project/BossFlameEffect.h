//=====================================
// @brief　ボス炎エフェクト
//=====================================
#pragma once

class Effect;
class BossFlameEffect : public Effect
{
public:
	BossFlameEffect(const int _effectResourceHandle);//コンストラクタ
	~BossFlameEffect() {}//デストラクタ

	void Update();
	void SetInfoToPlayTheEffect() override;
	void SetPosition(const VECTOR _position);
private:
};

