//=====================================
// @brief　獣ボスレベル上昇エフェクト
//=====================================
#pragma once

class Effect;
class BeastRaiseLevelEffect : public Effect
{
public:
	 BeastRaiseLevelEffect(const int _effectResourceHandle);//コンストラクタ
	~BeastRaiseLevelEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
	short	useFrameIndex;		//使用するフレーム番号
};

