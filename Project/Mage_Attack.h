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
	AttackCapsuleColliderData*  collider;						//�U���R���C�_�[
	short						frameIndexUsedCapsuleDirection1;//�U���J�v�Z�������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�U���J�v�Z�������߂邽�߂̃t���[���ԍ�
	float						startAnimationPlayTime;			//�A�j���[�V�����̍ŏ��̍Đ�����
};

