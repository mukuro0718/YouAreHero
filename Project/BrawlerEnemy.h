//====================================================
// @brief �f��ł̋ߐڍU��������G
//====================================================
#pragma once

class Enemy;
class BrawlerEnemy : public Enemy
{
public:
	 BrawlerEnemy(const int _indentNum, const int _bossType);//�R���X�g���N�^
	~BrawlerEnemy();					//�f�X�g���N�^

	void		Initialize		 () override;		//������
	void		Finalize		 () override;		//�㏈��
	void		Update			 () override;		//�X�V
	const void	DrawCharacterInfo() const override;	//�`��
	void		PlayAnimation	 ();				//�A�j���[�V�����̍Đ�

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
		ATTACK,			//�U��
		REACTION,		//�U�����󂯂����̃��A�N�V����
		SWAY,			//���
	};

	/*getter/setter*/
	const bool GetIsAttack	 ()const override;	//�R�E�Q�L������
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

