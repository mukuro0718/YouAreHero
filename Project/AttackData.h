//=======================================================
// @brief �U���f�[�^�N���X
//=======================================================
#pragma once

class AttackData
{
public:
			 AttackData();	//�R���X�g���N�^
	virtual ~AttackData(){}	//�f�X�g���N�^

	VECTOR	hitPosition;			//�q�b�g���W
	bool	isDoHitCheck;			//�����蔻������邩
	bool	isHitAttack;			//�U��������������
	int		damage;					//�_���[�W
	int		hitStopTime;			//�q�b�g�X�g�b�v����
	int		hitStopType;			//�q�b�g�X�g�b�v�̎��
	int		hitStopDelay;			//�q�b�g�X�g�b�v�f�B���C
	float	slowFactor;				//�X���E
	int		reactionType;			//���A�N�V�����̎��
	float	blockStaminaConsumption;//�u���b�N���̏���X�^�~�i
	/*
	HACK:
	�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A�L�����N�^�[���ɗ^������̂ɂȂ�
	�U�����̃q�b�g�X�g�b�v�́A�U���N���X�Őݒ肷��B
	*/
};

