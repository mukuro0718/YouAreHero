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

	void Update() override;//更新
private:
	float	yOffset;			//Yオフセット
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
};

