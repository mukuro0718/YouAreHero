//=====================================
// @brief　プレイヤーの攻撃ヒットエフェクト
//=====================================
#pragma once

class Effect;
class PlayerImpactEffect : public Effect
{
public:
	PlayerImpactEffect(const int _effectResourceHandle);//コンストラクタ
	~PlayerImpactEffect() {}//デストラクタ

		  void Update() override;		//更新
	const void Draw	 () const override;	//描画
private:
	/*定数*/
	const int MAX_EFFECT_NUM;//エフェクトの最大数

	/*メンバ変数*/
	VECTOR		 scale;				 //拡大率
	vector<int>	 playingEffectHandle;//再生エフェクトハンドル
	vector<int>	 frameCount;//再生エフェクトハンドル
	vector<bool> isPlay;			 //再生フラグ
};
