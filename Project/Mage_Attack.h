//=====================================================
// @brief �U���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Mage_Attack : public ActionNode
{
public:
	 Mage_Attack();//�R���X�g���N�^
	~Mage_Attack();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*  collider;						//�U���R���C�_�[
};

