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
		DEMON_PANDA,
	};
	//画像の種類
	enum class ImageType
	{		
		BOSS_HP_BAR_FRAME	,
		HP_TABLE			,
		BOSS_HP_BAR			,
		PLAYER_HP_BAR		,
		PLAYER_STAMINA_BAR	,
		PRESS_BUTTON_LOGO	,
		TITLE_LOGO			,
		QUEST_CLEAR			,
		QUEST_FAILED		,
		A_BUTTON			,
		B_BUTTON			,
		X_BUTTON			,
		Y_BUTTON			,
		LEFT_STICK			,
		RIGHT_STICK			,
		PUSH_RIGHT_STICK	,
		RIGHT_TRIGGER		,
		RIGHT_BUMPER		,
		BUTTON_SET_1		,
		BUTTON_SET_2		,
		LEFT_STICK_INPUT	,
		POTION				,
		POTION_TABLE		,
		ICON_LOCK_ON		,
		BACK_GROUND			,
		SHADOW				,
		SELECT_BACK			,
		SELECT_ICON_TABLE	,
		QUEST_IMAGE_1		,
		QUEST_IMAGE_2		,
		QUEST_IMAGE_3		,
		QUEST_IMAGE_4		,
		SELECT_FRAME		,
		SELECT_PROV_DECIDE	,
		SELECT_IMAGE_TABLE	,
		SELECT_DRAW_RECT	,

		OPERATION_TABLE		,

		BOSS_NORMAL_TEXTURE	,
		BOSS_ANGRY_TEXTURE	,
		BOSS_TIRED_TEXTURE	,
	};						  
	//フォントのタイプ
	enum class FontType
	{
		BOSS_NAME,
		MINTYO_50_32,
		MINTYO_150_32,
		ITEM_NUM,
		SELECT_HEADER,
		SELECT_QUEST,
		SELECT_ACTION,
		BUTTON_OPERATION,
	};
	enum class FontName
	{
		batman,
		Aihara,
	};
	//音のタイプ
	enum class SoundType
	{
		TITLE_BGM,
		BATTLE_BGM,
		
		CONFIRMATION,
		CANCEL,
		CHANGE_SELECT_ICON,
		
		SWORD_SLASH_1,//combo1,2
		SWORD_SLASH_2,//combo3
		SWORD_SLASH_3,//skill
		SWORD_SWING_1,
		SWORD_SWING_2,
		SWORD_SWING_3,
		PLAYER_WALK,
		PLAYER_RUN,
		SHIELD_BLOCK,
		HEAL_MAGIC,
		PLAYER_KNOCKDOWN,
		PLAYER_ROLL,

		MONSTER_LIGHT_ATTACK,
		MONSTER_HEAVY_ATTACK,
		MONSTER_SWING_1,
		MONSTER_SWING_2,
		MONSTER_SWING_3,
		MONSTER_BREATH,
		MONSTER_ROAR,
		MONSTER_FOOTSTEPS,

		LUXURIO_EXPLOSION,
		LUXURIO_SUPER_NOVA,
		LUXURIO_ROAR,
		LUXURIO_CHARGE,
		LUXURIO_WING,
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

