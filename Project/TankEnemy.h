//====================================================
// @brief �^���N
//====================================================
#pragma once

class Enemy;
class TankEnemy : public Enemy
{
public:
	TankEnemy ();//�R���X�g���N�^
	~TankEnemy();//�f�X�g���N�^

	void		Initialize		 () override;		//������
	void		Finalize		 () override;		//�㏈��
	void		Update			 () override;		//�X�V
	const void	DrawCharacterInfo() const override;	//�`��
	void		PlayAnimation	 ();				//�A�j���[�V�����̍Đ�
	void		SetSpownPosition(const int _indentNum, const int _bossType);

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		DYING,			//�_�C
		IDLE,			//�ҋ@
		RUN,			//����
		WALK_FRONT,		//����
		WALK_BACK,		//����
		WALK_LEFT,		//����
		WALK_RIGHT,		//����
		WEAK_ATTACK,	//�U��
		STRONG_ATTACK,	//�U��
		REACTION,		//�U�����󂯂����̃��A�N�V����
		GUARD,			//�K�[�h
		GUARD_REACTION,	//�K�[�h���A�N�V����
	};

	/*getter/setter*/
	const bool GetIsAttack()const override;	//�R�E�Q�L������
	void SetAttackCount() override {}		//�U���R���{�̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 4.0f; //�e�̃T�C�Y
	static constexpr float LOCKON_OFFSET = 5.0f;//���b�N�I���I�t�Z�b�g

	/*�����o�ϐ�*/
	int indentNum;
	int bossType;
};

