//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class BossAction;
class Boss : public Enemy
{
public:
	Boss();//�R���X�g���N�^
	~Boss();//�f�X�g���N�^

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();					//�A�j���[�V�����̍Đ�



	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		 DYING			= 0, //�f�X1
		 IDLE			= 1, //�ҋ@0
		 ROAR			= 2, //���K14
		 WALK			= 3, //����2
		 WALK_LEFT		= 4, //����3
		 WALK_RIGHT		= 5, //����4

		 SLASH_1		= 6, //�X���b�V��10
		 SLASH_2		= 7, //�X���b�V��5
		 STAB			= 8, //�˂��h���U��11
		 ROTATE_SLASH	= 9, //��]�X���b�V��7
		 PUNCH			= 10,//�p���`6
		 SLASH_COMBO_1	= 11,//�X���b�V���R���{�P8
		 SLASH_COMBO_2	= 12,//�X���b�V���R���{�Q9
		 JUMP_ATTACK	= 13,//�W�����v�A�^�b�N15
		 DOWN			= 14,//�_�E��16
		 DOWN_UP		= 15 //�_�E������N���オ��17
	};
	//�U���̎��
	enum class AttackType
	{
		NONE		  = -1,
		SLASH_1		  = 0, //�X���b�V�� 10
		SLASH_2		  = 1, //�X���b�V�� 5
		STAB		  = 2, //�˂��h���U�� 11
		ROTATE_SLASH  = 3, //��]�X���b�V�� 7
		PUNCH		  = 4, //�p���` 6
		SLASH_COMBO_1 = 5, //�X���b�V���R���{�P 8
		SLASH_COMBO_2 = 6, //�X���b�V���R���{�Q 9
		JUMP_ATTACK	  = 7, //�W�����v�A�^�b�N 15
	};

	/*getter/setter*/
	const bool		 GetIsAttack		()const override;										//�R�E�Q�L������
	const AttackType GetPrevAttackType	()const					 { return this->prevAttack; }	//�O�̃A�^�b�N�^�C�v�̎擾
		  void		 SetAttackType		(const AttackType _type) { this->prevAttack = _type; }	//�U���̎�ނ̃Z�b�g
		  void		 SetAttackCount		() override;											//�U���R���{�̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float LOCKON_OFFSET = 10.0f;//���b�N�I���I�t�Z�b�g
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	static constexpr int   COUNT_NUM	 = 6;	 //�t���[���J�E���g�̐�
	//��{���
	static constexpr unsigned int DYING	= (1 << 0);//�f�X
	static constexpr unsigned int IDLE	= (1 << 1);//�ҋ@
	static constexpr unsigned int ROAR	= (1 << 2);//���K
	static constexpr unsigned int WALK	= (1 << 3);//����
	static constexpr unsigned int REST	= (1 << 4);//�x�e
	//�U��
	static constexpr unsigned int SLASH_1		= (1 << 5);//�X���b�V���P
	static constexpr unsigned int SLASH_2		= (1 << 6);//�X���b�V���Q
	static constexpr unsigned int STAB			= (1 << 7);//�˂��h��
	static constexpr unsigned int ROTATE_SLASH	= (1 << 8);//��]�X���b�V��
	static constexpr unsigned int PUNCH			= (1 << 9);//�p���`
	static constexpr unsigned int SLASH_COMBO_1	= (1 << 10);//�X���b�V���R���{�P
	static constexpr unsigned int SLASH_COMBO_2	= (1 << 11);//�X���b�V���R���{�Q
	static constexpr unsigned int JUMP_ATTACK	= (1 << 12);//�W�����v�A�^�b�N

	static constexpr unsigned int MASK_ATTACK = SLASH_1 | SLASH_2 | ROTATE_SLASH |
												STAB | PUNCH | SLASH_COMBO_1 | SLASH_COMBO_2 | JUMP_ATTACK;
	static constexpr unsigned int MASK_ALL	  = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*�񋓑�*/
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		SLASH			= 0,
		STAB			= 1,
		ROTATE_PUNCH	= 2,
		REST			= 3,
	};
	enum class ActionType
	{
		NONE			= -1,
		DYING			= 0,//�f�X
		IDLE			= 1,//�ҋ@
		ROAR			= 2,//���K
		WALK			= 3,//����
		REST			= 4,//�x�e

		SLASH_1			= 5,//�X���b�V��
		SLASH_2			= 6,//�X���b�V��
		STAB			= 7,//�˂��h���U��
		ROTATE_SLASH	= 8,//��]�X���b�V��
		PUNCH			= 9,//�p���`
		SLASH_COMBO_1	= 10,//�X���b�V���R���{�P
		SLASH_COMBO_2	= 11,//�X���b�V���R���{�Q
		JUMP_ATTACK		= 12,//�W�����v�A�^�b�N
	};

	/*���������֐�*/
	void ChangeState	();//��Ԃ̕ύX
	void UpdateBossState();//�{���Ԃ̍X�V

	/*�����o�ϐ�*/
	std::map<int, unsigned int>	actionTypeMap;			//�A�N�V�����^�C�v
	std::vector<BossAction*>	parameters;				//�A�N�V�����p�����[�^�[
	AttackType					prevAttack;				//�O�̍U��
	int							nowAction;				//���݂̃A�N�V����
	int							normalTexture;			//�ʏ�̃e�N�X�`��
	int							angryTexture;			//�{���Ԏ��̃e�N�X�`��
	int							tiredTexture;			//����Ԏ��̃e�N�X�`��
};

