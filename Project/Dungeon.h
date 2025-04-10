//==========================================
// @brief �_���W�������N���X
//==========================================
#pragma once

class ModelColliderData;
class Dungeon abstract
{
public:
			 Dungeon();//�R���X�g���N�^
	virtual ~Dungeon();//�f�X�g���N�^

	virtual void	   Initialize() abstract;//������
	virtual void	   Update	 () abstract;//�X�V
	virtual const void Draw		 () const;	 //�`��

	/*getter*/
	const int	GetModelHandle()const;
		  void	OffIsDoHitCheck();

protected:
	int				modelHandle;//���f���n���h��
	ModelColliderData*	collider;	//�R���C�_�[
};

