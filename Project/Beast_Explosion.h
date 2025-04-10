//=====================================================
// @brief �����U��
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackSphereColliderData;
class Beast_Explosion : public ActionNode
{
public:
	 Beast_Explosion();//�R���X�g���N�^
	~Beast_Explosion();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	const void	Draw		()const;											//�`��
private:
	short						roarSoundStartCount;			//���K�̉��J�n�t���[��
	short						effectStartCount;				//�G�t�F�N�g�J�n�t���[��
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedSpherePosition;	//�J�����̒��S�����߂邽�߂̃t���[���ԍ�
	AttackSphereColliderData*	collider;						//�U���R���C�_�[
};

