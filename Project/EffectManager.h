#pragma once
#include "Singleton.h"


class Effect;
class BossFlameEffect;
class EffectManager : public Singleton<EffectManager>
{
public:
	/*EffectManager��Singleton���t�����h��*/
	friend class Singleton<EffectManager>;

	virtual ~EffectManager();//�f�X�g���N�^

	/*�񋓑�*/
	//�G�t�F�N�g�̎��
	enum class EffectType
	{
		BOSS_IMPACT			 = 0, //�{�X�̍U���������������̃G�t�F�N�g
		BOSS_ROAR			 = 1, //�{�X�̍U���������������̃G�t�F�N�g
		PLAYER_GUARD_HIT	 = 2, //�v���C���[�̃K�[�h�����������Ƃ��̃G�t�F�N�g
		PLAYER_HEAL			 = 3, //�v���C���[���񕜂����Ƃ��̃G�t�F�N�g
		PLAYER_IMPACT		 = 4, //�v���C���[�̍U���������������̃G�t�F�N�g
		BEAST_BREATH		 = 5, //�b�{�X�u���X�G�t�F�N�g
		BEAST_BACKING_BREATH = 6, //�b�{�X��ރu���X�G�t�F�N�g
		BEAST_WEAK_BREATH	 = 7, //�b�{�X��u���X�G�t�F�N�g
		BEAST_EXPLOSION		 = 8, //�b�{�X�����G�t�F�N�g
		BEAST_SUPER_NOVA	 = 9, //�b�{�X�唚���G�t�F�N�g
		BEAST_RAISE_LEVEL	 = 10,//�b�{�X���x���㏸�G�t�F�N�g
		BEAST_EXPLOSION_RANGE= 11,//�b�{�X�����͈̓G�t�F�N�g
		DRAGON_BREATH		 = 12,//�h���S���{�X�u���X�G�t�F�N�g
		MAGE_LASER			 = 13,
	};

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	
	void OnIsEffect (const EffectType _type);							//�G�t�F�N�g�Đ��t���O�𗧂Ă�
	void SetPosition(const EffectType _type, const VECTOR _position);	//���W�̐ݒ�
	void SetRotation(const EffectType _type, const VECTOR _rotation);	//��]���̐ݒ�
private:
	/*���������֐�*/
	EffectManager();//�R���X�g���N�^
		
	/*�����o�ϐ�*/
	std::vector<Effect*> effect;
	int frameTime;//�����܂łɂ�����������
};

