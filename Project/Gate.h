//======================================================
// @brief �Q�[�g�N���X
// �_���W��������{�X�̃}�b�v�֐؂�ւ��邽�߂̏������s��
// ���f���̓h�A
//======================================================
#pragma once

class Gate
{
public:
	 Gate();//�R���X�g���N�^
	~Gate();//�f�X�g���N�^

		  void Initialize(const int _bossType);	//������
		  void Update	 ();					//�X�V
	const void Draw		 ()const;				//�`��
private:
	/*�萔*/
	const float ACTION_LENGTH;//�Q�[�g�A�N�V�������\�ȋ���

	/*�����o�ϐ�*/
	int		modelHandle;	//���f���n���h��
	VECTOR	position;		//���W
	bool	isNearPlayer;	//�v���C���[���߂��ɂ��邩
};

