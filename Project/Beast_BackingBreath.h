//=====================================================
// @brief ���ɉ�����Ȃ���u���X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_BackingBreath : public ActionNode
{
public:
	 Beast_BackingBreath();//�R���X�g���N�^
	~Beast_BackingBreath();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	const void	Draw		()const;											//�`��
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*	collider;						//�U���R���C�_�[
	float						breathLength;					//�u���X�̒���
};

