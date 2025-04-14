//=====================================
// @brief　獣ボス弱ブレスエフェクト
//=====================================
#pragma once

class Effect;
class MageLaserEffect : public Effect
{
public:
	MageLaserEffect(const int _effectResourceHandle);//コンストラクタ
	~MageLaserEffect() {}//デストラクタ

	void Update() override;//更新
private:
	int		playingEffectHandle;//再生エフェクトハンドル
	bool	isPlay;				//対応するエフェクトを再生しているか
};

