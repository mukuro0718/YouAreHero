//=====================================
// @brief　獣ボス大爆発エフェクト
//=====================================
#pragma once

class Effect;
class BeastSuperNovaEffect : public Effect
{
public:
	 BeastSuperNovaEffect(const int _effectResourceHandle);//コンストラクタ
	~BeastSuperNovaEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR	scale;				//拡大率
	int		playingEffectHandle;//再生エフェクトハンドル
	short	useIndexNum;		//使用するフレームの番号
};

