//================================================================
// @brief �A�Z�b�g�̃��[�h�N���X
//================================================================
#pragma once
#include "Singleton.h"

class LoadingAsset : public Singleton<LoadingAsset>
{
public:
	friend class Singleton<LoadingAsset>;

	/*�񋓑�*/
	//���f���̎��
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
	//�摜�̎��
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
	//�t�H���g�̃^�C�v
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
	//���̃^�C�v
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
	//�G�t�F�N�g
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
	const int GetModel(const ModelType _index) { return this->modelHandle[static_cast<int>(_index)]; }//���f���̎擾
	const int GetImage(const ImageType _index) { return this->imageHandle[static_cast<int>(_index)]; }//�摜�̎擾
	const int GetFont (const FontType _index)  { return this->fontHandle [static_cast<int>(_index)]; }//�t�H���g�̎擾
	const int GetSound(const SoundType _index) { return this->soundHandle[static_cast<int>(_index)]; }//�T�E���h�n���h���̎擾
	const int GetEffect(const EffectType _index) { return this->effectHandle[static_cast<int>(_index)]; }//�G�t�F�N�g�n���h���̎擾
private:
	/*���������֐�*/
			 LoadingAsset();//�R���X�g���N�^
	virtual ~LoadingAsset();//�f�X�g���N�^

	std::vector<int> modelHandle;//���f���n���h��
	std::vector<int> imageHandle;//�摜�n���h��
	std::vector<int> fontHandle ;//�t�H���g�n���h��
	std::vector<int> soundHandle;//�T�E���h�n���h��
	std::vector<int> effectHandle;//�G�t�F�N�g�n���h��
};

