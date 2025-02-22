//================================================
// @brief �L�����N�^�[�f�[�^�N���X
//================================================
#pragma once

class CharacterData 
{
public:
			 CharacterData();	//�R���X�g���N�^
	virtual ~CharacterData(){}	//�f�X�g���N�^

	/*�����o�ϐ�*/
	VECTOR	hitPosition;	//�U���q�b�g���W
	bool	isHit;			//�U���Ƀq�b�g������
	bool	isGuard;		//�K�[�h�t���O
	bool	isInvinvible;	//���G�t���O
	int		hp;				//HP
	float   stamina;		//�X�^�~�i
	int		reactionType;	//���A�N�V�����^�C�v
	int		hitStopTime;	//�q�b�g�X�g�b�v�̎���
	int		hitStopType;	//�q�b�g�X�g�b�v�̎��
	int		hitStopDelay;	//�q�b�g�X�g�b�v�f�B���C
	float	slowFactor;		//�X���E
	float	defensivePower; //�h���
	float	damage;			//�_���[�W
	/*
	HACK:
	�����ł̃q�b�g�X�g�b�v�n�̕ϐ��́A
	�L�����N�^�[���ōs���q�b�g�X�g�b�v�̕ϐ��ł���
	*/
};

