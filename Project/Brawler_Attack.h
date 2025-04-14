//=====================================================
// @brief �U���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Brawler_Attack : public ActionNode
{
public:
	 Brawler_Attack();//�R���X�g���N�^
	~Brawler_Attack();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�U���J�v�Z�������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�U���J�v�Z�������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*  collider;						//�U���R���C�_�[
};

