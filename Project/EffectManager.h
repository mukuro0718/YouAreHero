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
		BOSS_IMPACT		 = 0,//ボスの攻撃が当たった時のエフェクト
		BOSS_ROAR		 = 1,//ボスの攻撃が当たった時のエフェクト
		PLAYER_GUARD_HIT = 2,//プレイヤーのガードが成功したときのエフェクト
		PLAYER_HEAL		 = 3,//プレイヤーが回復したときのエフェクト
		PLAYER_IMPACT	 = 4,//プレイヤーの攻撃が当たった時のエフェクト
		PLAYER_CHARGE	 = 5,//プレイヤーの溜め攻撃エフェクト
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
};

