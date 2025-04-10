//=====================================================
// @brief ��u���X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_WeakBreath : public ActionNode
{
public:
	 Beast_WeakBreath();//�R���X�g���N�^
	~Beast_WeakBreath();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	const void	Draw		()const;											//�`��
private:
	/*�萔*/
	const int FIX_ROTATE_COUNT;//��]�␳��

	/*�����o�ϐ�*/
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*	collider;						//�U���R���C�_�[
	float						breathLength;					//�u���X�̒���
};

