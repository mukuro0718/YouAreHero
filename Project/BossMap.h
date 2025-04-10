//===========================================
// @brief �n�ʃN���X
// �n�ʂ̕`����s��
//===========================================
#pragma once

class ModelColliderData;
class BossMap
{
public:
	BossMap();//�R���X�g���N�^
	~BossMap();//�f�X�g���N�^

		  void	Initialize		();		//������
		  void	Finalize		();		//�㏈��
		  void	Update			();		//�X�V
	const void	Draw			()const;//�`��

	/*getter*/
	const int	GetModelHandle()const;
		  void	OnIsDoHitCheck();
private:
	/*�����o�ϐ�*/
	ModelColliderData*	collider;	//�R���C�_�[
	int					modelHandle;//���f���n���h��
};

