#pragma once
#include "Singleton.h"

class Sound;
class SoundEffect;
class Bgm;
class SoundManager : public Singleton<SoundManager>
{
public:
	/*EffectManager��Singleton���t�����h��*/
	friend class Singleton<SoundManager>;


	/*�񋓑�*/
	//BGM�̎��
	enum class BgmType
	{
		TITLE_BGM,
		BATTLE_BGM,
	};
	//�G�t�F�N�g�̎��
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

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	
	void OnIsPlayBgm	(const BgmType _type);		//�G�t�F�N�g�Đ��t���O�𗧂Ă�
	void OnIsPlayEffect	(const EffectType _type);
	void OffIsPlayBgm	(const BgmType _type);		//�G�t�F�N�g�Đ��t���O������
	void OffIsPlayEffect(const EffectType _type);

	const bool GetCheckBGMSoundState(const BgmType _type);
	const bool GetCheckEffectSoundState(const EffectType _type);
private:
	/*���������֐�*/
	 SoundManager();//�R���X�g���N�^
	~SoundManager();//�f�X�g���N�^
		
	/*�����o�ϐ�*/
	vector<Bgm*>		bgms;	//BGM
	vector<SoundEffect*>effects;	//�T�E���h�G�t�F�N�g

	int frameTime;//�����܂łɂ�����������
};

