//=====================================================
// @brief �E���U���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_RightFootAttack : public ActionNode
{
public:
	 Beast_RightFootAttack();//�R���X�g���N�^
	~Beast_RightFootAttack();//�f�X�g���N�^

	NodeState  Update()override;//�X�V
	const void Draw	 ()const;   //�`��
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�J�v�Z�������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�J�v�Z�������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData* collider;						//�U���R���C�_�[
};
