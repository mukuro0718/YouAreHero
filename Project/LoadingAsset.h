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
		BOSS_HP_BAR_FRAME	= 0,
		HP_TABLE			= 1,
		BOSS_HP_BAR			= 2,
		PLAYER_HP_BAR		= 3,
		PLAYER_STAMINA_BAR	= 4,
		PRESS_BUTTON_LOGO	= 5,
		TITLE_LOGO			= 6,
		A_BUTTON			= 7,
		B_BUTTON			= 8,
		X_BUTTON			= 9,
		Y_BUTTON			= 10,
		LEFT_STICK			= 11,
		RIGHT_STICK			= 12,
		PUSH_RIGHT_STICK	= 13,
		RIGHT_TRIGGER		= 14,
		RIGHT_BUMPER		= 15,
		POTION				= 16,
		POTION_TABLE		= 17,
		BACK_GROUND			= 18,
		SHADOW				= 19,
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
		PLAYER_CHARGE_ATTACK,
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

