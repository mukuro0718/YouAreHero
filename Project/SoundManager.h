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
	};

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
	
	void OnIsPlayBgm(const BgmType _type);		//エフェクト再生フラグを立てる
	void OnIsPlayEffect(const EffectType _type);
	void OffIsPlayBgm(const BgmType _type);		//エフェクト再生フラグを下す
	void OffIsPlayEffect(const EffectType _type);
private:
	/*内部処理関数*/
	 SoundManager();//コンストラクタ
	~SoundManager();//デストラクタ
		
	/*メンバ変数*/
	vector<Bgm*>		bgms;	//BGM
	vector<SoundEffect*>effects;	//サウンドエフェクト

	int frameTime;//処理までにかかった時間
};

