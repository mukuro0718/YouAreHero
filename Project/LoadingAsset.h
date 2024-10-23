//================================================================
// @brief アセットのロードクラス
//================================================================
#pragma once
#include "Singleton.h"

class LoadingAsset : public Singleton<LoadingAsset>
{
public:
	friend class Singleton<LoadingAsset>;

	/*列挙体*/
	//モデルの種類
	enum class ModelType
	{
		FINALY_BOSS_STAGE,
		PLAYER,
		ENEMY,
		BOT,
		SKYDOME,
	};
	//画像の種類
	enum class ImageType
	{
		EMPTY_ORB,
		HEAL_ICON,
		HP_ORB,
		PAUSE_ICON,
		
		BOSS_HP_BAR,
		PLAYER_HP_BAR,
		PLAYER_STAMINA_BAR,
		BAR_TABLE,

		GAME_CLEAR,
		GAME_OVER,
		PRESS_A,
		TITLE_LOGO,
		WINDOW,
		CONTINUE,
		END,
		NONE_STAR,
		SILVER_STAR,
		GOLD_STAR,

		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		LT_BUTTON,
		PRESS_B_BUTTON,
		PRESS_X_BUTTON,
		PRESS_Y_BUTTON,
		PRESS_LT_BUTTON,
		SHADOW,
		BACK_GROUND,
		POTION,
		POTION_TABLE,
		HP_BAR_FRAME,
		A_BUTTON,
		PRESS_A_BUTTON,

		MUTANT_NEW_TEXTURE,
		MUTANT_PREV_TEXTURE,
	};
	//フォントのタイプ
	enum class FontType
	{
		MINTYO_80_32,
		MINTYO_50_32,
		MINTYO_100_32,
	};
	enum class FontName
	{
		batman,
		Aihara,
	};
	//音のタイプ
	enum class SoundType
	{

	};
	//エフェクト
	enum class EffectType
	{
		BOSS_IMPACT,
		BOSS_FLAME,
		PLAYER_GUARD_HIT,
		PLAYER_HEAL,
		PLAYER_IMPACT,
		PLAYER_JUST_GUARD,
	};

	/*getter*/
	const int GetModel(const ModelType _index) { return this->modelHandle[static_cast<int>(_index)]; }//モデルの取得
	const int GetImage(const ImageType _index) { return this->imageHandle[static_cast<int>(_index)]; }//画像の取得
	const int GetFont (const FontType _index)  { return this->fontHandle [static_cast<int>(_index)]; }//フォントの取得
	const int GetSound(const SoundType _index) { return this->soundHandle[static_cast<int>(_index)]; }//サウンドハンドルの取得
	const int GetEffect(const EffectType _index) { return this->effectHandle[static_cast<int>(_index)]; }//エフェクトハンドルの取得
private:
	/*内部処理関数*/
			 LoadingAsset();//コンストラクタ
	virtual ~LoadingAsset();//デストラクタ

	std::vector<int> modelHandle;//モデルハンドル
	std::vector<int> imageHandle;//画像ハンドル
	std::vector<int> fontHandle ;//フォントハンドル
	std::vector<int> soundHandle;//サウンドハンドル
	std::vector<int> effectHandle;//エフェクトハンドル
};

