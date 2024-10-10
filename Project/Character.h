//=============================================================
// @brief �L�����N�^�[�x�[�X�N���X
//=============================================================
#pragma once

class ColliderData;
class CharacterData;
class Rigidbody;
class BitFlag;
class Animation;
class Character abstract
{
public:
	Character();//�R���X�g���N�^
	virtual ~Character();//�f�X�g���N�^
	
	virtual void		Initialize		 () abstract;//������
	virtual void		Finalize		 () abstract;//�㏈��
	virtual void		Update			 () abstract;//�X�V
	virtual const void	DrawCharacterInfo() const abstract;
	const void			Draw			 () const;//�`��

	/*getter/setter*/
	virtual const bool			 GetIsAttack		 ()const abstract;
			const CharacterData& GetCharacterData	 ()const;
			const Rigidbody&	 GetRigidbody		 ()const;
			const int			 GetHP				 ()const;
			const int			 GetModelHandle		 ()const { return this->modelHandle; }
			const bool			 GetIsChangeAnimation()const;
			const bool			 GetIsAlive			 ()const { return this->isAlive; }
				  void			 OffIsAlive			 () { this->isAlive = false; }
	/*��[�Պ֐�*/
	float  Lerp(const float _start, const float _end, const float _percent);	//��[�Պ֐�
	VECTOR Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//��[�Պ֐�
protected:

	/*�����o�ϐ�*/
	Animation*		animation;	//�A�j���[�V����
	BitFlag*		state;		//���
	ColliderData*	collider;	//�R���C�_�[
	int				modelHandle;//���f���n���h��
	bool			isAlive;	//�L�����N�^�[���������Ă��邩
	bool			isGround;	//�n�ʂɓ������Ă���
	float			speed;		//���x
	int				entryInterval;
	bool isDraw;
	bool isInitialize;
};
