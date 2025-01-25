//=====================================
// @brief　獣ボスブレスエフェクト
//=====================================
#pragma once

class Effect;
class DragonBreathEffect : public Effect
{
public:
	 DragonBreathEffect(const int _effectResourceHandle);//コンストラクタ
	~DragonBreathEffect() {}//デストラクタ

	void Update() override;//更新
private:
	VECTOR			scale;				//拡大率
	vector<int>		playingEffectHandle;//再生エフェクトハンドル
	vector<VECTOR>	position;			//座標
	vector<VECTOR>	velocity;			//座標
	vector<bool>	isPlay;				//対応するエフェクトを再生しているか
	short			useFrameIndex1;		//使用するフレームの番号
	short			useFrameIndex2;		//使用するフレームの番号
	short			maxEffectNum;		//エフェクトの数
	short			useEffectNum;		//使用しているエフェクトの数
	float			moveSpeed;			//エフェクトの移動スピード
	float			maxDistance;		//初期位置からの最大距離
};

