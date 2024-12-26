#pragma once
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	/*EffectManagerとSingletonをフレンドか*/
	friend class Singleton<SoundManager>;


	/*列挙体*/
	//エフェクトの種類
	enum class EffectType
	{
		PLAYER_COMBO_1,
		PLAYER_COMBO_2,
		PLAYER_COMBO_3,
		PLAYER_S_ATTACK,
	};

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
	
	void OnIsPlay(const EffectType _type, const bool isTop);							//エフェクト再生フラグを立てる
private:
	/*内部処理関数*/
	 SoundManager();//コンストラクタ
	~SoundManager();//デストラクタ
		
	/*メンバ変数*/
	std::vector<int> sound;//サウンドハンドル
	std::vector<bool> isPlay;//再生するか
	std::vector<int> playType;//再生携帯
	std::vector<bool> isTopPosition;//再生位置を音データの先頭に移動するかどうか

	int frameTime;//処理までにかかった時間
};

