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

	void Update() override;//更新
private:
	float	yOffset;			//Yオフセット
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
};

