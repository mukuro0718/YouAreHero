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
		COLL_ARENA,
		PLAYER,
		ENEMY,
		SKYDOME,
		BEAST,
		DRAGON,
	};
	//画像の種類
	enum class ImageType
	{		
		BOSS_HP_BAR_FRAME		= 0,
		HP_TABLE				= 1,
		BOSS_HP_BAR				= 2,
		PLAYER_HP_BAR			= 3,
		PLAYER_STAMINA_BAR		= 4,
		PRESS_BUTTON_LOGO		= 5,
		TITLE_LOGO				= 6,
		A_BUTTON				= 7,
		B_BUTTON				= 8,
		X_BUTTON				= 9,
		Y_BUTTON				= 10,
		LEFT_STICK				= 11,
		RIGHT_STICK				= 12,
		PUSH_RIGHT_STICK		= 13,
		RIGHT_TRIGGER			= 14,
		RIGHT_BUMPER			= 15,
		BUTTON_SET_1			= 16,
		BUTTON_SET_2			= 17,
		POTION					= 18,
		POTION_TABLE			= 19,
		BACK_GROUND				= 20,
		SHADOW					= 21,

		SELECT_BACK				= 22,
		SELECT_ICON_TABLE		= 23,
		QUEST_IMAGE_1			= 24,
		QUEST_IMAGE_2			= 25,
		QUEST_IMAGE_3			= 26,
		QUEST_IMAGE_4			= 27,
		SELECT_FRAME			= 28,
		SELECT_PROV_DECIDE		= 29,
		SELECT_IMAGE_TABLE		= 30,
		SELECT_DRAW_RECT		= 31,

		OPERATION_WALK			= 32,
		OPERATION_RUN			= 33,
		OPERATION_AVOID			= 34,
		OPERATION_GUARD			= 35,
		OPERATION_DEVALUTION	= 36,
		OPERATION_ROUNDING_UP	= 37,
		OPERATION_DIVE_CUTTING	= 38,
		OPERATION_ROTARY_CUTTER	= 39,
		OPERATION_USE_ITEM		= 40,
		OPERATION_SWORD_DELIVERY= 41,
		OPERATION_SWORD_DRAWING	= 42,
		BOSS_NORMAL_TEXTURE		= 43,
		BOSS_ANGRY_TEXTURE		= 44,
		BOSS_TIRED_TEXTURE		= 45,
		LEFT_STICK_INPUT		= 46,
	};						  
	//フォントのタイプ
	enum class FontType
	{
		MINTYO_80_64,
		MINTYO_50_32,
		MINTYO_150_32,
		MINTYO_30_32,
		SELECT_HEADER,
		SELECT_QUEST,
		SELECT_ACTION,
	};
	enum class FontName
	{
		batman,
		Aihara,
	};
	//音のタイプ
	enum class SoundType
	{
		SWORD_SLASH_1,//combo1,2
		SWORD_SLASH_2,//combo3
		SWORD_SLASH_3,//skill
	};
	//エフェクト
	enum class EffectType
	{
		BOSS_IMPACT,
		BOSS_ROAR,
		BOSS_FLAME,
		PLAYER_GUARD_HIT,
		PLAYER_HEAL,
		PLAYER_IMPACT,
		PLAYER_JUST_GUARD,
		BEAST_BACKING_BREATH,
		BEAST_BREATH,
		BEAST_EXPLOSION,
		BEAST_PARTS_FIRE,
		BEAST_RAISE_LEVEL,
		BEAST_SUPER_NOVA,
		BEAST_WEAK_BREATH,
		BEAST_EXPLOSION_RANGE,
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

