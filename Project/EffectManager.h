#pragma once
#include "Singleton.h"


class Effect;
class BossFlameEffect;
class EffectManager : public Singleton<EffectManager>
{
public:
	/*EffectManagerとSingletonをフレンドか*/
	friend class Singleton<EffectManager>;

	virtual ~EffectManager();//デストラクタ

	/*列挙体*/
	//エフェクトの種類
	enum class EffectType
	{
		BOSS_IMPACT			 = 0, //ボスの攻撃が当たった時のエフェクト
		BOSS_ROAR			 = 1, //ボスの攻撃が当たった時のエフェクト
		PLAYER_GUARD_HIT	 = 2, //プレイヤーのガードが成功したときのエフェクト
		PLAYER_HEAL			 = 3, //プレイヤーが回復したときのエフェクト
		PLAYER_IMPACT		 = 4, //プレイヤーの攻撃が当たった時のエフェクト
		BEAST_BREATH		 = 5, //獣ボスブレスエフェクト
		BEAST_BACKING_BREATH = 6, //獣ボス後退ブレスエフェクト
		BEAST_WEAK_BREATH	 = 7, //獣ボス弱ブレスエフェクト
		BEAST_EXPLOSION		 = 8, //獣ボス爆発エフェクト
		BEAST_SUPER_NOVA	 = 9, //獣ボス大爆発エフェクト
		BEAST_RAISE_LEVEL	 = 10,//獣ボスレベル上昇エフェクト
		BEAST_EXPLOSION_RANGE= 11,//獣ボス爆発範囲エフェクト
	};

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
	
	void OnIsEffect (const EffectType _type);							//エフェクト再生フラグを立てる
	void SetPosition(const EffectType _type, const VECTOR _position);	//座標の設定
private:
	/*内部処理関数*/
	EffectManager();//コンストラクタ
		
	/*メンバ変数*/
	std::vector<Effect*> effect;
	int frameTime;//処理までにかかった時間
};

