#pragma once
#include "Singleton.h"

class Sound;
class SoundEffect;
class Bgm;
class SoundManager : public Singleton<SoundManager>
{
public:
	/*EffectManagerとSingletonをフレンドか*/
	friend class Singleton<SoundManager>;


	/*列挙体*/
	//BGMの種類
	enum class BgmType
	{
		TITLE_BGM,
		BATTLE_BGM,
	};
	//エフェクトの種類
	enum class EffectType
	{
		CONFIRMATION_SOUND,
		CANCEL_SOUND,
		CHANGE_SELECT_ICON,

		PLAYER_COMBO_1,
		PLAYER_COMBO_2,
		PLAYER_COMBO_3,
		PLAYER_S_ATTACK,
		PLAYER_COMBO_1_SWING,
		PLAYER_COMBO_2_SWING,
		PLAYER_COMBO_3_SWING,
		PLAYER_S_ATTACK_SWING,
		PLAYER_WALK,
		PLAYER_RUN,
		PLAYER_BLOCK,
		PLAYER_HEAL,
		PLAYER_KNOCKDOWN,
		PLAYER_ROLL,

		MONSTER_LIGHT_ATTACK,
		MONSTER_HEAVY_ATTACK,
		MONSTER_SWING_1,
		MONSTER_SWING_2,
		MONSTER_SWING_3,
		MONSTER_ROAR,
		MONSTER_BREATH,
		MONSTER_FOOTSTEPS,

		LUXURIO_EXPLOSION,
		LUXURIO_SUPER_NOVA,
		LUXURIO_ROAR,
		LUXURIO_CHARGE,
		LUXURIO_WING,
	};

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
	
	void OnIsPlayBgm	(const BgmType _type);		//エフェクト再生フラグを立てる
	void OnIsPlayEffect	(const EffectType _type);
	void OffIsPlayBgm	(const BgmType _type);		//エフェクト再生フラグを下す
	void OffIsPlayEffect(const EffectType _type);

	const bool GetCheckBGMSoundState(const BgmType _type);
	const bool GetCheckEffectSoundState(const EffectType _type);
private:
	/*内部処理関数*/
	 SoundManager();//コンストラクタ
	~SoundManager();//デストラクタ
		
	/*メンバ変数*/
	vector<Bgm*>		bgms;	//BGM
	vector<SoundEffect*>effects;	//サウンドエフェクト

	int frameTime;//処理までにかかった時間
};

